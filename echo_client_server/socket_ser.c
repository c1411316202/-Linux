#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m) ;\
		exit(EXIT_FAILURE) ; \
	} while(0) 

int main(void)
{
	int listenfd ;
	if( (listenfd = socket( PF_INET , SOCK_STREAM , IPPROTO_TCP )) < 0 )
		ERR_EXIT("socket");
	struct sockaddr_in servaddr ;
	memset( &servaddr ,0,sizeof(servaddr) ) ;
	servaddr.sin_family = PF_INET ;
	servaddr.sin_port = htons(6666) ;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1") ;
	if( (bind( listenfd , ( struct sockaddr*) &servaddr , sizeof(servaddr) ) ) < 0 ) 
		ERR_EXIT("bind");
	if(listen( listenfd , SOMAXCONN ) < 0 ) 
		ERR_EXIT("listen");
	struct sockaddr_in peeraddr ;
	socklen_t peerlen = sizeof(peeraddr) ;
	int conn ;
	if( ( conn = accept( listenfd , ( struct sockaddr* )&peeraddr , &peerlen ) ) < 0 )
		ERR_EXIT("accept") ;
	char recvbuf[1024] ;
	while(1)
	{
		memset( recvbuf , 0 , sizeof(recvbuf) ) ;
		int ret = read(conn , recvbuf , sizeof(recvbuf) ) ;
		fputs( recvbuf , stdout) ;
		write( conn , recvbuf , ret ) ;
	}
	close(conn) ;
	close(listenfd) ;
	return 0 ;
}
