#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 5050

int main(){
	
	int sockfd, connfd, new;
	struct sockaddr_in servaddr, clientaddr; //contains address information
	
	//getting the socket file descriptor
	//int socket(int domain, int type, int protocol);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd == -1){
		perror("Error: ");
		exit(0);
	}
	
	printf("\nSocket creation successful!");
	
	
	
	//setting all the bits of servaddr to 0
	
	bzero(&servaddr, sizeof(servaddr));
	
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT); //htons-host to network short
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY) ;//host to network long
	
	//assosciate socket with port on local machine
	//int bind(int sockfd, struct sockaddr *my_addr, int addrlen)
	
	if( bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
		perror("Error ");
		exit(0);
	}

	printf("\nSocket binding successful!");

	
	//ready to accept client connection requests
	//only for stream sockets
	//int listen(int sockfd, int backlog)
	
	if( listen(sockfd, 5) == -1){
		perror("Error ");
	}

	printf("\nServer listening for client requests...");
	
	
	//connect to a specified location/client
	//int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
	
	
	
	printf("\nServer-Client connection successfully established!");
	
	
	//accept connection. creates a new file descriptor for the connection
	//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
	
	new = sizeof(clientaddr);
	
	connfd = accept(sockfd, (struct sockaddr *)&clientaddr, &new);
	
	if( connfd == -1){
		perror("Error: ");
		exit(0);
	}
	
	printf("\nClient accepted...");
	
	
	char buffer[100];
	
	read(connfd, buffer, sizeof(buffer));
	printf("\nMessage from the client: %s", buffer);
	
	close(sockfd);

	return 0;
}
