#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_EMPLOYEES 1000

struct __attribute__((__packed__))
employee_t {
    int id;
    char firstName[64];
    char lastName[64];
    float income;
    bool isManager;
};

int employee_create(struct employee_t *self) {
    static int numEmployees = 0;
    numEmployees++;

    self->id        = numEmployees;
    self->income    = 0.0;
    self->isManager = false;
    for (size_t i = 0; i < 64; i++) {
        self->firstName[i] = 0;
        self->lastName[i]  = 0;
    }

    return numEmployees;
}

int main(int argc, char const *argv[]) {
    int x   = 3;
    int *pX = &x;

    printf("value: %d\nStore at: %p\n", *pX, pX);  // *pX--> deference a pointer
    printf("value: %d\nStore at: %p\n", x, &x);

    struct employee_t Fuckgencio;
    employee_create(&Fuckgencio);

    printf("%d\n", Fuckgencio.id);

    // Using dynamic memory to store muchos employees:
    int n                        = 4;  // Reservamos espacio para 4 empleados:
    struct employee_t *employees = malloc(sizeof(struct employee_t) * n);
    if (employees == NULL) {
        return -1;
    }

    for (size_t i = 0; i < n; i++) {
        int id = employee_create(&employees[i]);
        printf("%d\n", employees[i].id);
    }

    free(employees);
    employees = NULL;  // It's not required, but always a smart idea to set used pointers to NULL.
                       // By setting pointers to NULL, we avoid what is called a dangling pointer,
                       // or a pointer that points to memory that is no longer valid.

    return 0;
}
