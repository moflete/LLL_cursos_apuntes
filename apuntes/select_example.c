#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_CLIENTS 256
#define PORT 8080
#define BUFF_SIZE 4096
#define BACKLOG 10

typedef enum {
    STATE_NEW          = 0,
    STATE_CONNECTED    = 1,
    STATE_DISCONNECTED = 2,
    STATES
} state_e;

// Structure/object to hold the client state
typedef struct {
    int fd;
    state_e state;
    char buffer[BUFF_SIZE];

} clientState_t;

clientState_t clientStates[MAX_CLIENTS];

void init_clients() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clientStates[i].fd    = -1; // -1 Indicates FREE SLOT
        clientStates[i].state = STATE_NEW;
        memset(clientStates[i].buffer, '\0', BUFF_SIZE);
    }
}

int find_free_slot() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clientStates[i].fd == -1) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char const *argv[]) {

    int listen_fd; // Listening socket
    int conn_fd;   // Connection socket
    int nfds;      // Number of file descriptors tracking
    int freeSlot;
    struct sockaddr_in server_addr, client_addr;

    socklen_t client_len = sizeof(client_addr);

    // What file descriptors are readable/writable
    fd_set read_fds, write_fds;

    // Initialize client states:
    init_clients();

    // Create listening socket
    if (-1 == (listen_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(PORT);

    /* BIND */
    if (-1 == (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))) {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    /* LISTEN */
    if (listen(listen_fd, BACKLOG) == -1) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Clear FD sets
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);

        // Add the listening socket to the read set
        FD_SET(listen_fd, &read_fds);
        nfds = listen_fd + 1;

        // Add Active connections to the read set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clientStates[i].fd != -1) {
                FD_SET(clientStates[i].fd, &read_fds);
                if (clientStates[i].fd >= nfds)
                    nfds = clientStates[i].fd + 1;
            }
        }

        // Wait for activity on one of the sockets
        if (-1 == (select(nfds, &read_fds, &write_fds, NULL, NULL))) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Check for new connections
        if (FD_ISSET(listen_fd, &read_fds)) {
            if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
                perror("accept");
                continue;
            }

            printf("New connection from %s:%d\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Find free slot
            freeSlot = find_free_slot();
            if (freeSlot == -1) {
                printf("Server full: Closing new connection.\n");
                close(conn_fd);
            } else {
                clientStates[freeSlot].fd    = conn_fd;
                clientStates[freeSlot].state = STATE_CONNECTED;
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clientStates[i].fd != -1 &&
                FD_ISSET(clientStates[i].fd, &read_fds)) {

                ssize_t bytes_read = read(clientStates[i].fd,
                                          &clientStates[i].buffer,
                                          BUFF_SIZE);

                if (bytes_read <= 0) {
                    close(clientStates[i].fd);
                    clientStates[i].fd    = -1;
                    clientStates[i].state = STATE_DISCONNECTED;
                    printf("Client disconnected or error\n");
                } else {
                    printf("Received data from client: %s\n", clientStates[i].buffer);
                }
            }
        }
    }
    return 0;
}
