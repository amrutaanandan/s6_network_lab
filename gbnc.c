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

void gobacknclient(int sockfd, int no_frames, int window_size){

	char buffer[MAX];
	
	int ack, i, w_high = window_size - 1, w_low = 0, flag = 0, j, n, k;
	
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) == -1 ){
		perror("[-] Set sockopt(SO_RCVTIMEO) failed\n");
		exit(1);
	}
	
	//send initial frames
	for(i=0; i < no_frames && i <= w_high; i++){//i denotes current frame no
		bzero(buffer, MAX);
		snprintf(buffer, MAX, "%d", i);
		k = send(sockfd, buffer, sizeof(buffer), 0);
		printf("Frame %d sent\n", i);
	}
	
	while(1){
		if(w_high - w_low != window_size - 1 && flag == 0 && i != no_frames){
			bzero(buffer, MAX);
			snprintf(buffer, MAX, "%d", i);
			k = send(sockfd, buffer, sizeof(buffer), 0);
			printf("Frame %d sent\n", i);
			w_high++;
			i++ ;
		}
		
		flag = 0;
		bzero(buffer, MAX);
		n = recv(sockfd, buffer, MAX, 0);
		ack = atoi(buffer);
		
		if(n > 0){
			if (ack + 1 == no_frames){
		        printf("Acknowledgement received: %d\nExit\n", ack);
		        bzero(buffer, sizeof(buffer));
		        strcpy(buffer, "Exit");
		        k = send(sockfd, buffer, sizeof(buffer), 0);
		        break;
        	}
		    if (ack == w_low)
		    {
		        w_low++;
		        printf("Acknowledgement received: %d\n", ack);
		    }
		    else{
				printf("Acknowledgement not received for %d\nResending frames\n", w_low);
				for (j = w_low; j < no_frames && j < w_low + window_size; j++)
				{
				    bzero(buffer, sizeof(buffer));
				    snprintf(buffer, sizeof(buffer), "%d", j);
				    k = send(sockfd, buffer, sizeof(buffer), 0);
				    printf("Frame %d sent\n", j);
				}
				flag = 1;
			}
		}
	}
}

void main()
{
    int sockfd, connfd, f, w;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("[-] Socket creation failed\n");
        exit(0);
    }
    else
        printf("[+] Socket successfully created\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0){
        printf("[-] Connection with the server failed\n");
        exit(0);
    }
    else
        printf("[+] Connected to the server\n");
    printf("Enter the number of frames: ");
    scanf("%d", &f);
    printf("Enter the window size: ");
    scanf("%d", &w);
    gobacknclient(sockfd, f, w);
    close(sockfd);
}
