#include <sys/types.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
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
    
    read(fd, buf, sizeof(proto_hdr_t) + sizeof(int));

    hdr->type = ntohl(hdr->type);
    hdr->len  = ntohs(hdr->len);
    // Unpack the data received
    int *data = (int *)&hdr[1];
    *data = ntohl(*data);

    if (hdr->type != PROTO_HELLO) {
        printf("Protocol mismatch, failing.\n");
        return;
    }

    if (*data != 1) {
        printf("Protocol version mismatch failing\n");
        return;
    }

    printf("Server connected, protocol v1.\n");
}


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

    handle_client(fd);
    
    close(fd);

    return 0;
}
