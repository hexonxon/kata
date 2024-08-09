
/*
 *	Socket class
 *	Encapsulates some socket types implementation
 *	With Socket interface
 *	
 *********************************************************************/

#ifndef SOCKET_H_
#define SOCKET_H_

//#define DEBUG_SOCKET

#include <sys/socket.h>
#include <netinet/in.h> /*	sockaddr_in	*/
#include <arpa/inet.h>	/*	inet_x	*/
#include <string.h>	/*	memset	*/
#include <stdio.h>	/*	close	*/
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <netdb.h> 

//Cause i hate casts
#define SA			struct sockaddr
#define IPV4_FAMILY(saddr)	((struct sockaddr_in *)saddr)->sin_family
#define IPV6_FAMILY(saddr)	((struct sockaddr_in6 *)saddr)->sin6_family
#define IPV4_PORT(saddr)	((struct sockaddr_in *)saddr)->sin_port
#define IPV6_PORT(saddr)	((struct sockaddr_in6 *)saddr)->sin6_port
#define IPV4_ADDR(saddr)	((struct sockaddr_in *)saddr)->sin_addr
#define IPV6_ADDR(saddr)	((struct sockaddr_in6 *)saddr)->sin6_addr

/*
 *	Syntactic sugar.
 */
typedef struct proto_sockaddr {

	//WARNING: Structs share memory, so take care
	union {
		struct sockaddr_in saddr;
		struct sockaddr_in6 saddr6;
	} sockaddr_u;

#define ipv4		sockaddr_u.saddr
#define ipv6		sockaddr_u.saddr6
#define ipv4_family	sockaddr_u.saddr.sin_family
#define ipv6_family	sockaddr_u.saddr6.sin6_family
#define ipv4_port	sockaddr_u.saddr.sin_port
#define ipv6_port	sockaddr_u.saddr6.sin6_port
#define ipv4_addr	sockaddr_u.saddr.sin_addr.s_addr
#define ipv6_addr	sockaddr_u.saddr6.sin6_addr

} proto_sockaddr_t;



//Abstract
class Socket {
public:

	virtual bool InitSocket(int, int, int)		=0;
	virtual bool Connect(const char *, int, int&) 	=0;
	virtual bool Close()				=0;
	virtual bool Bind(int)				=0;
	virtual bool Listen(int)			=0;
	virtual bool Accept(int&)			=0;	

	virtual ~Socket() {};

protected:
	Socket() {};

};


class BSDSocket : public Socket {
public:

	BSDSocket()
	:sockfd(-1) {};
	
	bool InitSocket(int family, int type, int protocol);
	bool Connect(const char *host, int port, int &connfd);
	bool Close();
	bool Bind(int portno);
	bool Listen(int listenq);
	bool Accept(int &connfd);

	~BSDSocket();

	/*
		Named parameters
		TODO
	*/

	BSDSocket &setLinger(bool is) {
		return *this;
	}

	BSDSocket &setBroadcast(bool is) {
		return *this;
	}

	BSDSocket &setKeepalive(bool is) {
		return *this;
	}

	BSDSocket &setReuseaddr(bool is) {
		return *this;
	}

	BSDSocket &setReuseport(bool is) {
		return *this;
	}

protected:

	struct addrinfo *resolve(const char *hostname);
	
	void proto_setport(struct sockaddr *saddr, int value) {

		switch(saddr->sa_family) {
		case AF_INET:	{ IPV4_PORT(saddr) = htons(value); break; }
		case AF_INET6:	{ IPV6_PORT(saddr) = htons(value); break; }
		default:	{ error("Set Port: Unknown adress family\n"); }
		} 

	}	 

	const char *proto_inet_ntop(struct sockaddr *saddr, char *buf, size_t len) {

		switch(saddr->sa_family) {
		case AF_INET:	return inet_ntop(AF_INET, &IPV4_ADDR(saddr), buf, len);
		case AF_INET6:	return inet_ntop(AF_INET6, &IPV6_ADDR(saddr), buf, len);
		default:	{ error("InetNtop: Unknown adress family\n"); return NULL;}
		} 

	}

	void error(const char *msg) {
		std::cout << "Socket error: " << msg << std::endl;
	}
 
	void error() {
		char str[128];
		int errno_copy = errno; 
		error(strerror_r(errno_copy, str, sizeof(str)));
	}

private:
	int sockfd;

};

//TODO
class UnixDomainSocket : public Socket {
public:
protected:
private:

};


#endif /*SOCKET_H_*/
