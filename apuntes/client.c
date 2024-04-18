#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 5555 
#define BACKLOG 0

int main (int argc, char *argv[]) {
    if (argc != 2){
        printf("Usage: %s <ip address of the server host\n", argv[0]);
        return 0;
    }

    struct sockaddr_in serverAddress = {0};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(PORT);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("error");
        return fd;
    }

    // CONNECT
    if (connect(fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1){
        perror("connect");
        close(fd);
        return 0;
    }

    close(fd);
    
    return 0;
}
