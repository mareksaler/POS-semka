#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main( int argc, char * argv[]) {

    int sockfd, newsockfd, portno, n;
    struct sockaddr_in serv_addr, client_addr;

    char buffer[256];

    socklen_t len;
    if (argc < 2) {
        printf("Malo argumentov\n");
        exit(1);
    }

    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0) {
        printf("Error socket\n");
        exit(1);
    }

    // struktura pre server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // bind socket to the address and port
    if (bind(sockfd, (struct  sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error binding socket\n");
    }

    // listen
    listen(sockfd, 5);

    len = sizeof(client_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &len);
    if (newsockfd < 0) {
        printf("Error accepting request\n");
        exit(1);
    }

    while (1) {
        bzero(buffer, 256); // clear buffer

        //citaj data od clienta
        n = read(newsockfd, buffer, 256);

        if (n < 0) {
            printf("Error reading\n");
            exit(1);
        }

        // print client msg
        printf("Client --> %s\n", buffer);

        bzero(buffer, 256); // clear buffer

        // msg to client from server
        fgets(buffer, 256, stdin);
        n = write(newsockfd, buffer, 256);

        if(n<0) {
            printf("Error in writing\n");
            exit(1);
        }

        // detect end of conversation
        if (!strncmp("bye", buffer, 3)) {
            break;
        }

    }

    // close sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}
