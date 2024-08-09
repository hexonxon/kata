
/*
 *	TCPState class
 *	Encapsulates state changes in tcp connection process
 *	
 *********************************************************************/

#ifndef TCPSTATE_H
#define TCPSTATE_H

#include <stdint.h>
#include <stdlib.h>
#include "conn.h"

class TCPConnection;

/*
 * Abstract class
 */
 
class TCPState {
public:
	virtual void ActiveOpen(TCPConnection &context)	=0;
	virtual void PassiveOpen(TCPConnection &context)	=0;
	virtual void Close(TCPConnection &context)	=0;
	
	virtual size_t Read(TCPConnection &context, void *ptr, size_t num)		=0;
	virtual size_t Write(TCPConnection &context, const void *ptr, size_t num)	=0;

	virtual ~TCPState() {};

protected:
	TCPState() {};

	void error(const char *msg) {
		std::cout << msg << std::endl;
	}

	inline void SetState(TCPConnection &context, TCPState *state);
	inline tcpcontext_t &accessContext(TCPConnection &context);
	
};



/*
 * C: CLOSED
 * S: CLOSED
 */
 
class TCPStateClosed : public TCPState {
public:
	TCPStateClosed();
	
	void ActiveOpen(TCPConnection &context);
	void PassiveOpen(TCPConnection &context);
	void Close(TCPConnection &context);
	
	size_t Read(TCPConnection &context, void *ptr, size_t num);
	size_t Write(TCPConnection &context, const void *ptr, size_t num);

	~TCPStateClosed();
};

/*
 * C: CLOSED - SYN_SENT - ESTABLISHED 
 * S: CLOSED - LISTEN - SYN_RCVD - ESTABLISHED
 */
 
class TCPStateEstablished : public TCPState {
public:
	TCPStateEstablished(int _connfd);
	
	void ActiveOpen(TCPConnection &context);
	void PassiveOpen(TCPConnection &context);
	void Close(TCPConnection &context);

	size_t Read(TCPConnection &context, void *ptr, size_t num);
	size_t Write(TCPConnection &context, const void *ptr, size_t num);

	~TCPStateEstablished();

private:
	int connfd;
};

/*
 * C: --
 * S: CLOSED - LISTEN
 */
 
class TCPStateListen : public TCPState {
public:
	TCPStateListen();

	void ActiveOpen(TCPConnection &context);
	void PassiveOpen(TCPConnection &context);
	void Close(TCPConnection &context);

	size_t Read(TCPConnection &context, void *ptr, size_t num);
	size_t Write(TCPConnection &context, const void *ptr, size_t num);

	~TCPStateListen();
};

/*
 * C: ESTABLISHED - FIN_WAIT_1 - FIN_WAIT_2 - TIME_WAIT
 * S: ESTABLISHED - CLOSE_WAIT - LAST_ACK 
 */
class TCPStateTimewait : public TCPState {
public:
	TCPStateTimewait();

	void ActiveOpen(TCPConnection &context);
	void PassiveOpen(TCPConnection &context);
	void Close(TCPConnection &context);

	size_t Read(TCPConnection &context, void *ptr, size_t num);
	size_t Write(TCPConnection &context, const void *ptr, size_t num);

	~TCPStateTimewait();
};

#endif /*TCPSTATE_H*/
