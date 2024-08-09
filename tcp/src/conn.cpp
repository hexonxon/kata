   
/*
 *	TCPconnection class
 *	Wraps up sockets, states and exports control via strategy
 *	Implementation.
 *
 *********************************************************************/
#include "conn.h"

#include "state.h"

TCPConnection::TCPConnection(socktype_t socktype = SOCK_BSD, TCPStrategy *strategy = new TCPStrategy())
:state(new TCPStateClosed())
,strat(strategy) 
{

	InitData();
	data.strat = strat;

	switch(socktype) {
		case SOCK_BSD: {data.sock = new BSDSocket();break;}
		case SOCK_UNIX:{break;}
		default:{}
	}
	
	
}

TCPConnection::~TCPConnection() {
	Close();
	delete data.sock;
	delete state;
	delete strat;
}

/*
 *	Connection methods
 */

void TCPConnection::ActiveOpen() {
	state->ActiveOpen(*this);
}

void TCPConnection::ActiveOpen(char *host, uint32_t port) {
	SetHost(host);
	SetPort(port);
	ActiveOpen();
}

void TCPConnection::PassiveOpen() {
	state->PassiveOpen(*this);
}

void TCPConnection::PassiveOpen(uint32_t port, uint32_t listenq) {
	SetPort(port);
	SetListenQueue(listenq);
	PassiveOpen();
}

void TCPConnection::Close() {
	state->Close(*this);
}
	
size_t TCPConnection::Read(void *ptr, size_t num) {
	return state->Read(*this, ptr, num);
}

size_t TCPConnection::Write(const void *ptr, size_t num) {
	return state->Write(*this, ptr, num);
}

void TCPConnection::SetState(TCPState *_state) {
	if(_state != state) {
		delete state;
		state = _state;
	}
}
