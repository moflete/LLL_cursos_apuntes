#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <unistd.h>

#define PORT 5555 
#define BACKLOG 0

typedef enum {
    PROTO_HELLO,
} proto_type_e;

// TLV
typedef  struct {
    proto_type_e type; // Type of the packet
    unsigned short len;
} proto_hdr_t;

void handle_client(int fd) {
    char buf[4096] = {0}; // Make room for a buffer to send through the wire.
    proto_hdr_t *hdr = (proto_hdr_t *)buf;  // With this way, we can work with the buffer as named structure.

    hdr->type = htonl(PROTO_HELLO);
    // hdr->len = htons(sizeof(int)); // 4 bytes
    hdr->len = sizeof(int);         // 4 bytes
    int real_length = hdr->len;     // Save temporaly the real length for future use.
    hdr->len = htons(sizeof(int));  // 4 bytes + HostToNetworkShort


    // Aquí hacer el esfuerzo de entender estas dos lineas
    int *data = (int *)&hdr[1];
    *data =  htonl(1);

    write(fd, hdr, sizeof(proto_hdr_t) + real_length);
}


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
    while(1) {           
        int cfd = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientSize);
        if(cfd == -1){
            perror("accept");
            close(serverSocket);
            return -1;
        }

        handle_client(cfd);

        close(cfd);
    }
}
