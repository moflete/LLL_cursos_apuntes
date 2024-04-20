#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
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
#define NO_TIMEOUT -1

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

int find_slot_by_fd(int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clientStates[i].fd == fd) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {

    int listen_fd;
    int conn_fd;
    int freeSlot;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    struct pollfd fds[MAX_CLIENTS + 1];
    int nfds = 1;
    int opt  = 1;

    // Initialize client states:
    init_clients();

    // Create listening socket yoda-style
    if (-1 == (listen_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    if (-1 == (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))) {
        perror("setsockopt");
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
    if (-1 == (listen(listen_fd, BACKLOG))) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    memset(fds, 0, sizeof(fds));
    fds[0].fd     = listen_fd;
    fds[0].events = POLLIN;
    nfds          = 1;

    while (1) {
        int ii = 1;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clientStates[i].fd != -1) {
                fds[ii].fd     = clientStates[i].fd;
                fds[ii].events = POLLIN; // Offset by 1 for listen_fd
                ii++;
            }
        }

        // Wait for an event on one of the sockets
        int n_events = poll(fds, nfds, NO_TIMEOUT); // -1 means no timeout
        if (n_events == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // Check for new connections
        if (fds[0].revents & POLLIN) {
            if (-1 == (conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len))) {
                perror("accept");
                continue;
            }

            printf("New connection from %s:%d\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            freeSlot = find_free_slot();
            if (freeSlot == -1) {
                printf("Server is full: Closing connection\n");
                close(conn_fd);
            } else {
                clientStates[freeSlot].fd    = conn_fd;
                clientStates[freeSlot].state = STATE_CONNECTED;
                nfds++;
                printf("Slot %d has fd %d\n", freeSlot, clientStates[freeSlot].fd);
            }
            n_events--; //
        }

        // Check each client for read and write activity:
        for (int i = 0; i <= nfds && n_events > 0; i++) { // Start from 1 to skip listen_fd
            if (fds[i].revents & POLLIN) {
                n_events--;

                int fd             = fds[i].fd;
                int slot           = find_slot_by_fd(fd);
                ssize_t bytes_read = read(fd, &clientStates[slot].buffer,
                                          sizeof(clientStates[slot].buffer));
                if (bytes_read <= 0) {
                    // Connection closed or error
                    close(fd);
                    if (-1 == slot) {
                        printf("Tried to close an fd does doesnt exist?\n");
                    } else {
                        clientStates[slot].fd    = -1;
                        clientStates[slot].state = STATE_DISCONNECTED;
                        printf("Client disconnected or error");
                        nfds--;
                    }
                } else {
                    printf("Received data from client: %s\n", clientStates[slot].buffer);
                }
            }
        }
    }
}
