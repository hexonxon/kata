/*
 *	TCPStrategy class
 *	Strategy is used to specify client behaviour
 *	
 *********************************************************************/
#ifndef STRATEGY_H
#define STRATEGY_H

class TCPConnection;

class TCPStrategy {
public:
	virtual void OnConnect(TCPConnection &conn)	{};
	virtual void OnDisconnect(TCPConnection &conn)	{};
	
	virtual ~TCPStrategy(){};
	TCPStrategy() {};

protected:
	
	
};

#endif /*STRATEGY_H*/

