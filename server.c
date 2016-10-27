#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

int main()
{
    struct sockaddr_in server;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(14022);

    int z = bind(sockfd, (struct sockaddr *) &server, sizeof(server));
    if(z<0){printf("err in bind\n");}
    int len = sizeof(server);
    char c ;
    recvfrom(sockfd, &c, 1, 0, (struct sockaddr *) &server, &len);
    int x, y;
    printf("Enter position: ");
    scanf("%d %d", &x, &y);

    sendto(sockfd, &x, 4, 0, (struct sockaddr *) &server, sizeof(server));
    sendto(sockfd, &y, 4, 0, (struct sockaddr *) &server, sizeof(server));
    return 0;
}