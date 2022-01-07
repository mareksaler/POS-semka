#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char * argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in server_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3) {
        printf("Malo argumentov\n");
        exit(1);
    }

    portno = 4892;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // check if socket is created properly
    if (sockfd < 0) {
        printf("Error opening socket\n");
        exit(1);
    }

    // vytvorenie struckruty pre server address
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_port = htons(portno);
//    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    server = gethostbyname(argv[1]); // pouzijeme funkciu gethostbyname na ziskanie informacii o pocitaci, ktoreho hostname je v prvom argumente
    if (server == NULL)                     //gethostbyname - zisti specificke info o servery
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&server_addr, sizeof(server_addr)); //(bzero - vynuluje hodnotu) vunulujeme a zinicializujeme adresu, na ktoru sa budeme pripajat
    server_addr.sin_family = AF_INET;
    bcopy(              //kopiruje konkretne byty na specificku poziciu
            (char*)server->h_addr,  //zoebrie zo server h_addr
            (char*)&server_addr.sin_addr.s_addr, //nakopiruje ju na poziciu sin_addr a s_addr
            server->h_length
    );
    server_addr.sin_port = htons(atoi(argv[2])); // nastavenie portu //htons - transformacia z little endian na big endian



    // connect to server
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed\n");
        exit(1);
    }

    while (1) {
        bzero(buffer, 256); // clear buffer

        // write data to server
        fgets(buffer, 256, stdin);
        n = write(sockfd, buffer, strlen(buffer));

        if (n < 0) {
            printf("Error writin\n");
            exit(1);
        }

        // read back from server
        bzero(buffer, 256); // clear buffer

        n = read(sockfd, buffer, 256);

        if (n<0) {
            printf("Error reading\n");
            exit(1);
        }

        // print
        printf("Server --> %s\n", buffer);

        // detect end of conversation
        if (!strncmp("bye", buffer, 3)) {
            break;
        }

    }

    close(sockfd);

    return 0;
}
