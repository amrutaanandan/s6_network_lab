#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#define MAX 80
#define PORT 8080

int main(){
	
	int sockfd, connfd, new;
	struct sockaddr_in servaddr, clientaddr;
	
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd == -1){
		perror("Error: ");
		exit(0);
	}
	
	printf("\nSocket creation successful!");
	
	
	
	//setting all the bits of servaddr to 0
	
	bzero(&servaddr, sizeof(servaddr));
	
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY) ;

	
	if( bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
		perror("Error ");
		exit(0);
	}

	printf("\nSocket binding successful!");
	
	if( listen(sockfd, 5) == -1){
		perror("Error ");
	}

	printf("\nServer listening for client requests...");
		
	
	printf("\nServer-Client connection successfully established!");
	
	
	new = sizeof(clientaddr);
	
	connfd = accept(sockfd, (struct sockaddr *)&clientaddr, &new);
	
	if( connfd == -1){
		perror("Error: ");
		exit(0);
	}
	
	printf("\nClient accepted.");

	
	//selective repeat
	
	int start=0, end, windowsize, ch, err_frame, i;
	
	read(connfd, &windowsize, sizeof(windowsize));
	end = windowsize - 1;
	
	while(1){
		read(connfd, &ch, sizeof(ch));
		
		if(ch == -1){
			printf("\nExit\n");
			break;
		}
		
		switch(ch){
		
			//frame received ack sent
			case 0: 
				for(i=start; i<=end; i++){
					printf("\nFrame %d received", i);
					sleep(1);
					printf("\nAcknowledgement for %d sent", i);
					sleep(1);
				}
				printf("\n");
				break;
			
			case 1:{
				read(connfd, &err_frame, sizeof(err_frame));
				
				for(i=start; i<=end; i++){
					if( i == err_frame ){
    					printf("\nFrame %d not received", err_frame);
    					sleep(1);
    				}
    				else{
    					printf("\nFrame %d received", i);
						sleep(1);
						printf("\nAcknowledgement for %d sent", i);
						sleep(1);
    				}
				}
    			printf("\nLost frame %d received", err_frame);
    			printf("\nAcknowledgement for %d sent", err_frame);
    			printf("\n");
				
				break;
			}
		}
		
		start = end + 1;
		end = end = windowsize - 1;
	}
	
	close(sockfd);

	return 0;
}
