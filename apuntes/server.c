#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <unistd.h>

#define PORT 5555 
#define BACKLOG 0

int main () {

    struct sockaddr_in serverAddress = {0};
    struct sockaddr_in clientAddress = {0};
    int clientSize = 0;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1){
        perror("error");
        return serverSocket;
    }

    /* BIND */
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1){
        perror("bind");
        close(serverSocket);
        return -1;
    }
    /* LISTEN */
    if (listen(serverSocket, BACKLOG) == -1) {
        perror("listen");
        close(serverSocket);
        return -1;
    }
    /* ACCEPT */
    // Client File Descriptor
    int cfd = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientSize);
    if(cfd == -1){
        perror("accept");
        close(serverSocket);
        return -1;
    }
    close(cfd);
}
