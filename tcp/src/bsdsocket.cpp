
/*
 *	BSD Socket implementation
 *
 *********************************************************************/
#include "socket.h"


BSDSocket::~BSDSocket() {
	Close();
}

/*
 *	Init sockfd with socket call
 */
bool BSDSocket::InitSocket(int family, int socktype, int protocol) {
	if( (sockfd = socket(family, socktype, protocol)) < 0) {
		error();
		return false;
	} else {
		return true;
	}
}

/*
 *	Connect to hostname on port
 */
bool BSDSocket::Connect(const char *hostname, int port, int &connfd) {
	struct addrinfo *addrlist, *cur;
	cur = addrlist = resolve(hostname);

	while(cur != NULL) {
		
		proto_setport(cur->ai_addr, port);
	
		//Soft Error, try again.
		if(!InitSocket(cur->ai_family, cur->ai_socktype, cur->ai_protocol))
			continue;
		
		if((connect(sockfd, cur->ai_addr, cur->ai_addrlen)) == 0) {
			connfd = sockfd;
			freeaddrinfo(addrlist);
			return true;
		} else {
			error();
		}
		
		Close();
		cur = cur->ai_next;
	}

	freeaddrinfo(addrlist);
	return false;
}

/*
 *	Close the socket fd
 */
bool BSDSocket::Close() {
	if(sockfd == -1) return true;

	if(close(sockfd) == 0) {
		sockfd = -1;
		return true;
	} else {
		error();
		return false;
	}
}

/*
 *	Bind a socket
 */
bool BSDSocket::Bind(int portno) {
	proto_sockaddr_t addr;
	addr.ipv4_family = AF_INET;
	addr.ipv4_port = htons(portno);
	addr.ipv4_addr = htonl(INADDR_ANY);

	if(!InitSocket(AF_INET, SOCK_STREAM, 0)) return false;

	if(bind(sockfd, (SA *)&addr.ipv4, sizeof(addr.ipv4)) == 0) {
		return true;
	} else {
		error();
		return false;
	}
}

/*
 *	Make a listening socket
 */
bool BSDSocket::Listen(int listenq) {
	if(listen(sockfd, listenq) == 0) {
		return true;
	} else {
		error();
		return false;
	}
}

/*
 *	Try to get an established connection or lock while waiting
 */
bool BSDSocket::Accept(int &connfd) {
	struct sockaddr paddr;
	paddr.sa_family = AF_INET;

	if((connfd = accept(sockfd, NULL, NULL)) < 0) {
		error();
		return false;
	} else {
		return true;
	}


}

/*
 *	Get addrinfo list of the host
 */
struct addrinfo *BSDSocket::resolve(const char *hostname) {

	int retcode;
	struct addrinfo hint, *ailist;
	
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_flags = AI_ALL;
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;

	if( (retcode = getaddrinfo(hostname, NULL, &hint, &ailist)) != 0) {
		error(gai_strerror(retcode));
		return NULL;
	}

#ifdef DEBUG_SOCKET /* --------------------------------------------------------	*/
	for(struct addrinfo *cur = ailist; cur != NULL; cur = cur->ai_next) {

		char *family, *socktype, addr[INET6_ADDRSTRLEN];
	
		switch (cur->ai_family) {
		#include "./codegen/af_x.h"
		}

		switch (cur->ai_socktype) {
		#include "./codegen/sock_x.h"
		}

		proto_inet_ntop(cur->ai_addr, addr, sizeof(addr));
		
		std::cout << "Address info for " << hostname << std::endl
			  << "Family: " << family << std::endl
			  << "Socket Type: " << socktype << std::endl
			  << "Address: " << addr << std::endl << std::endl;

	}
#endif /* ---------------------------------------------------------------------	*/

	return ailist;
}




