#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>

#define PORT 5050
#define MAX 100

struct timeval timeout;

void gobacknserver(int connfd){

	char buffer[MAX];
	
	int frame_no, random_ack, ack, expected_frame = 0;
	
	while(1){
		
		bzero(buffer, MAX);
		recv(connfd, buffer, sizeof(buffer), 0); //recieve a frame
		
		if(strcmp("Exit", buffer) == 0){ //all frames sent, exit
			printf("Exit\n");
			break;
		}
		
		frame_no = atoi(buffer);
		
		if (frame_no != expected_frame){ //frame not expected frame
			printf("Frame %d discarded\nAcknowledgement sent: %d\n", frame_no, ack);
			bzero(buffer, MAX); //sizeof(buffer) = MAX
			snprintf(buffer, sizeof(buffer), "%d", ack);
			send(connfd, buffer, sizeof(buffer), 0);
			continue;
		}
		
		random_ack = rand()%3; //simulate a random scenario for the acknowledgement
		
		switch(random_ack){
			case 0:{
				break;
			}
			case 1:{ //ack with delay
				ack = frame_no;
				sleep(2);
				printf("Frame %d recieved\nAcknowledgement sent: %d\n", frame_no, ack); //send ack for previous frame
				bzero(buffer, MAX); //sizeof(buffer) = MAX
				snprintf(buffer, sizeof(buffer), "%d", ack);
				send(connfd, buffer, sizeof(buffer), 0);
				expected_frame = ack + 1; //if ack recieved successfully, update expected_frame value
				break;
			}
			case 2:{ //ack with no delay
				ack = frame_no;
				printf("Frame %d recieved\nAcknowledgement sent: %d\n", frame_no, ack);
				bzero(buffer, MAX); //sizeof(buffer) = MAX
				snprintf(buffer, sizeof(buffer), "%d", ack);
				send(connfd, buffer, sizeof(buffer), 0);
				expected_frame = ack + 1;
				break;
			}
		}
	}

}

void main(){
	struct sockaddr_in servaddr, cliaddr;
	int sockfd, connfd, length;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd == -1){
		perror("[-] Socket creation failed\n");
		exit(1);
	}
	
	printf("[+] Socket creation successful\n");
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
		perror("[-] Socket binding failed\n");
		exit(1);
	}
	printf("[+] Socket binding successful\n");
	
	if(listen(sockfd, 5) == -1){
		perror("[-] Socket listen failed\n");
		exit(1);
	}
	
	printf("[+] Socket listening\n");
	
	length = sizeof(servaddr);
	
	connfd = accept(sockfd, (struct sockaddr *)&servaddr, &length);
	
	if(connfd == -1){
		perror("[-] Socket failed to accept client\n");
		exit(1);
	}
	printf("[+] Socket accepted client\n");
	
	gobacknserver(connfd);
	close(sockfd);
}
