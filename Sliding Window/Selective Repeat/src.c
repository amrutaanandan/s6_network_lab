#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#define MAX 80
#define PORT 8080


void main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, clientaddr;
    
    srand(time(NULL));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully created!\n");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection with the server failed.\n");
        exit(0);
    }
    else
        printf("Connected to the server.\n");

    
    //selective repeat
    int start = 0, windowsize, end, totalframes, ch, err_frame, i;
    
    printf("\nEnter total number of frames and windowsize: ");
    scanf("%d%d", &totalframes, &windowsize);
    
    write(sockfd, &windowsize, sizeof(windowsize));
    
    end = windowsize-1;
    
    while(1){
    	if(end >= totalframes){
    		ch = -1;
    		write(sockfd, &ch, sizeof(ch));
    		break;
    	}
    	
    	ch = rand()%2;
    	write(sockfd, &ch, sizeof(ch));
    	
    	switch(ch){
    		
    		case 0: //frame ack not lost
    			for(i=start; i<=end; i++){
    				printf("\nFrame %d sent", i);
    				sleep(1);
    				printf("\nFrame %d acknowledged", i);
    				sleep(1);
    				
    			}
    			printf("\n");
    			break;
    		
    		case 1:
    			//generate lost frame no:
    			err_frame = rand()%(start + end - 1) + start;
    			write(sockfd, &err_frame, sizeof(err_frame));
    			
    			for(i=start; i<=end; i++){
    				if( i == err_frame ){
    					printf("\nFrame %d ack not received", err_frame);
    					sleep(1);
    				}
    				else{
    					printf("\nFrame %d sent", i);
						sleep(1);
						printf("\nFrame %d acknowledged", i);
						sleep(1);
    				}
    			}
    			
    			//resend lost frame
    			printf("\nResending lost frame %d", err_frame);
    			printf("\nFrame %d resent", err_frame);
    			printf("\n");
    			break;
    			
    	}
    	
    	start = end + 1;
    	end += windowsize - 1;
    }
    
    printf("\nExit\n");
    
    close(sockfd);
}

    

