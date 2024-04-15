#include <sys/types.h>  
#include <sys/socket.h>

#include <stdio.h>

int main () {

    int fd; // A file descriptor represents a file opened by OS to make operations over it.
    // fd = open("./mydata");   
    // 0 -> stdin
    // 1 -> stdout
    // 2 -> stderr
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("error");
        return fd;
    }
    
    printf("%d\n", fd);
    return 0;
}
