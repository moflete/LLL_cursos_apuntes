#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* wc -c my-db.db ---> gets size in bytes */

struct database_header_header_t {
    unsigned short version;
    unsigned short employees;
    unsigned int filelength;
};

int main(int argc, char const *argv[]) {
    struct database_header_header_t head = {0};
    struct stat dataBaseStatus           = {0};

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    int file_descriptor = open(argv[1], O_RDWR | O_CREAT, 0644);
    if (file_descriptor == -1) {
        perror("open");
        return -1;
    }

    if (read(file_descriptor, &head, sizeof(head)) != sizeof(head)) {
        perror("read");
        close(file_descriptor);
        return -1;
    }

    printf("DB Version: %u\n", head.version);
    printf("DB Number of Employees: %u\n", head.employees);
    printf("DB File Length: %u\n", head.filelength);

    if (fstat(file_descriptor, &dataBaseStatus) < 0) {
        perror("fstat");
        close(file_descriptor);
        return -1;
    }

    printf("DB File Length, reported by fstat: %lu\n", dataBaseStatus.st_size);

    if (dataBaseStatus.st_size != head.filelength) {
        printf("GET OUT OF HERE HACKER!\n");
        close(file_descriptor);
        return -2;
    }

    close(file_descriptor);
    return 0;
}
