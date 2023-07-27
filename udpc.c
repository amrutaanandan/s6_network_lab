#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 5050
#define MAX 1024

void main(){
	struct sockaddr_in servaddr, cliaddr;
	int sockfd, connfd, length, leng;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sockfd == -1){
		perror("Error ");
		exit(1);
	}
	
	printf("Socket created successfully\n");
	
	memset(&servaddr, 0 , sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	
	
	printf("Enter message to send to server:\n");
	char buffer[MAX];
	scanf("%s", buffer);
	

	sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	length = strlen(buffer);
	buffer[length] = '\0';
	
	close(sockfd);
	
}


