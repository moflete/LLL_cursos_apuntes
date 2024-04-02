#include <stdbool.h>
#include <stdio.h>
#define DEBUG 1
#define MAX_EMPLOYEES 1000

struct employee_t {
    int id;
    char firstName[64];
    char lastName[64];
    float income;
    bool isManager;
};

int main(int argc, char const *argv[]) {
#ifdef DEBUG
    printf("Filename: %s\nRunning program: %s\n", __FILE__, argv[0]);
#endif  // DEBUG

    struct employee_t employees[MAX_EMPLOYEES];

    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        employees[i].income    = 0;
        employees[i].isManager = false;

        printf("%d\n", (int)i);
    }
    return 0;
}