#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 5050
#define MAX 1024

void main(){

	struct sockaddr_in servaddr, cliaddr;
	int sockfd, connfd, client, n, leng;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sockfd == -1){
		perror("Error ");
		exit(1);
	}
	
	printf("Socket created successfully\n");
	
	bzero(&servaddr, sizeof(servaddr));
	
	memset(&servaddr, 0 , sizeof(servaddr));
	memset(&cliaddr, 0 , sizeof(cliaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
		perror("Error ");
		exit(1);
	}
	
	printf("Socket binded successfully\n");
	
	char buffer[MAX];
	
	printf("Message recieved from the client is: \n");
	
	leng = sizeof(servaddr);
	
	n = recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&servaddr, &leng);
	
	buffer[n] = '\0';	
	printf("Client message is: %s", buffer);
	
	close(sockfd);
}
