#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 5050


void main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, clientaddr;

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

    char buf1[100], buf2[100];

    printf("Enter message to send to server: ");
    scanf("%s", buf1);
    write(sockfd, buf1, sizeof(buf1));
    close(sockfd);
}

    

