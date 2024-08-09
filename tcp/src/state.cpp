
/*
 *	TCPState implementation
 *
 *********************************************************************/
#include "state.h"

inline void TCPState::SetState(TCPConnection &context, TCPState *state) {
	context.SetState(state);
}

inline tcpcontext_t &TCPState::accessContext(TCPConnection &context) {
	return context.GetData();
}


TCPStateClosed::TCPStateClosed() {
}

TCPStateClosed::~TCPStateClosed() {
}

//Just open a connection
void TCPStateClosed::ActiveOpen(TCPConnection &context) {
	int connfd;
	tcpcontext_t connd = accessContext(context);

	if(connd.sock->Connect(connd.host, connd.port, connfd)) {
		SetState(context, new TCPStateEstablished(connfd));
		connd.strat->OnConnect(context);
	} else { 
		TCPState::error("Error: On opening connection");
	}
}

//Bind - Listen - TCPLISTEN - ActiveOpen()
void TCPStateClosed::PassiveOpen(TCPConnection &context) {
	tcpcontext_t connd = accessContext(context);

	if(!connd.sock->Bind(connd.port)) {
		TCPState::error("Error: On binding socket");
		return;
	} 
	
	if(connd.sock->Listen(connd.listenq)) {
		SetState(context, new TCPStateListen());
		context.ActiveOpen();
	} else {
		TCPState::error("Error: On creating listening locket");
	}
}

//Nothing to close yet
void TCPStateClosed::Close(TCPConnection &context) {
	TCPState::error("Warning: Nothing to close\n");
	return;
}

//We can`t read from a closed connection
size_t TCPStateClosed::Read(TCPConnection &context, void *ptr, size_t num) {
	TCPState::error("Error: Reading from closed connection");
	return 0;
}

//We can`t write to a closed connection
size_t TCPStateClosed::Write(TCPConnection &context, const void *ptr, size_t num) {
	TCPState::error("Error: Writing to closed connection");
	return 0;
}

/* --------------------------------------------	*/

TCPStateEstablished::TCPStateEstablished(int _connfd)
:connfd(_connfd) {
}

TCPStateEstablished::~TCPStateEstablished() {
	connfd = -1;
}

//Hey, we`re connected! Close - ActiveOpen()
void TCPStateEstablished::ActiveOpen(TCPConnection &context) {
	TCPState::error("Warning: Closing current connection to open a new one\n");

	this->Close(context);
	context.ActiveOpen();
}

//Hey, we`re connected! Close - PassiveOpen()
void TCPStateEstablished::PassiveOpen(TCPConnection &context) {
	TCPState::error("Warning: Closing current connection to open a new one\n");
	
	this->Close(context);
	context.PassiveOpen();
}

//Close the connection
void TCPStateEstablished::Close(TCPConnection &context) {
	tcpcontext_t connd = accessContext(context);

	if(connd.sock->Close()) {
		SetState(context, new TCPStateClosed);
		connd.strat->OnDisconnect(context);
	} else {
		TCPState::error("Error: On closing established connection");
	}
}

//Return size_t bytes read
size_t TCPStateEstablished::Read(TCPConnection &context, void *ptr, size_t num) {
	size_t nleft = num;
	size_t nread = 0;
	char *p = (char *)ptr;

	while(nleft > 0) {
		if((nread = read(connfd, p, nleft)) > 0) { //Normal
			nleft -= nread;
			p += nread;
		} else if(nread < 0) {	//Error
			if(errno != EINTR) return -1;
		} else {	//EOF
			break;
		}
	}

	return (num - nleft);
}

//Return size_t bytes written
size_t TCPStateEstablished::Write(TCPConnection &context, const void *ptr, size_t num) {
	size_t nleft = num;
	size_t nwritten = 0;
	const char *p = (const char *)ptr;

	while(nleft > 0) {
		if((nwritten = write(connfd, p, nleft)) > 0) {
			nleft -= nwritten;
			p += nwritten;
		} else {
			if(errno != EINTR) return -1;
		}
	}

	return (num - nleft);
}


TCPStateListen::TCPStateListen() {
}

TCPStateListen::~TCPStateListen() {
}

//This means, that we`ve already created a listening socket and want to accept connection
void TCPStateListen::ActiveOpen(TCPConnection &context) {
	int connfd;
	tcpcontext_t connd = accessContext(context);

	while(connd.sock->Accept(connfd)) {
		SetState(context, new TCPStateEstablished(connfd));
		connd.strat->OnConnect(context);
	}

	TCPState::error("Error: On accept");
}

//Open new listening socket
void TCPStateListen::PassiveOpen(TCPConnection &context) {
	TCPState::error("Warning: Rebinding socket");

	this->Close(context);
	context.PassiveOpen();
}

//Stop listening
void TCPStateListen::Close(TCPConnection &context) {
	tcpcontext_t connd = accessContext(context);

	if(connd.sock->Close()) {
		SetState(context, new TCPStateClosed());
	} else {
		TCPState::error("Error: On closing listening socket");
	}
}

//We can`t read from a listening connection
size_t TCPStateListen::Read(TCPConnection &context, void *ptr, size_t num) {
	TCPState::error("Error: Reading from listening connection");
	return 0;
}

//We can`t write to a listening connection
size_t TCPStateListen::Write(TCPConnection &context, const void *ptr, size_t num) {
	TCPState::error("Error: Writing to listening connection");
	return 0;
}


TCPStateTimewait::TCPStateTimewait() {
}

TCPStateTimewait::~TCPStateTimewait() {
}

//Wait, then connect
void TCPStateTimewait::ActiveOpen(TCPConnection &context) {
	
}

//Wait, then listen
void TCPStateTimewait::PassiveOpen(TCPConnection &context) {
	
}

//Just wait
void TCPStateTimewait::Close(TCPConnection &context) {
	
}

//We can`t read from a waiting connection
size_t TCPStateTimewait::Read(TCPConnection &context, void *ptr, size_t num) {
	TCPState::error("Error: Reading from waiting connection");
	return 0;
}

//We can`t write to a waiting connection
size_t TCPStateTimewait::Write(TCPConnection &context, const void *ptr, size_t num) {
	TCPState::error("Error: Writing to waiting connection");
	return 0;
}
