#ifndef TCPCONN_H_
#define TCPCONN_H_
   
/*
 *	TCPconnection class
 *	Wraps up sockets, states and exports control via strategy
 ****************************************************************************/

#include "socket.h"
#include "strategy.h"

class TCPState;

// socket listen queue
const uint32_t DEFAULT_LISTENQ = 1024;

// tcp context info for export
typedef struct tcpcontext {
	Socket *sock;
	TCPStrategy *strat;
	char *host;
	char *peer_host;
	uint32_t port;
	uint32_t peer_port;
	uint32_t listenq;
} tcpcontext_t;

// type of socket
typedef enum {
	SOCK_BSD,
	SOCK_UNIX
} socktype_t;

class TCPConnection {
public:

	TCPConnection(socktype_t socktype, TCPStrategy *strategy);
	~TCPConnection();
	
	void ActiveOpen();
	void ActiveOpen(char *host, uint32_t port);
	void PassiveOpen();
	void PassiveOpen(uint32_t port, uint32_t listenq);
	void Close();
	size_t Read(void *ptr, size_t num);
	size_t Write(const void *ptr, size_t num);
		
	void SetHost(char *host) 		{data.host = host;}
	void SetPort(uint32_t port) 		{data.port = port;}
	void SetListenQueue(uint32_t listenq) 	{data.listenq = listenq;}

	const char *GetHost() 		{return data.host;}
	const char *GetRemoteHost() 	{return data.peer_host;}
	uint32_t GetPort() 		{return data.port;}
	uint32_t GetRemotePort() 	{return data.peer_port;}
	uint32_t GetListenQueue() 	{return data.listenq;}

protected:

	friend class TCPState;
	
	void SetState(TCPState *_state);
	
	tcpcontext_t &GetData() {return data;}
	void InitData() 	{memset(&data, 0, sizeof(data));}

private:

	TCPState *state;
	TCPStrategy *strat;

private:

	tcpcontext_t data;
};


#endif /*TCPCONN_H_*/
