#include <stdbool.h>
#include <stdio.h>
#define DEBUG 1
#define MAX_EMPLOYEES 1000

//
struct __attribute__((__packed__))
employee_t {
    int id;
    char firstName[64];
    char lastName[64];
    float income;
    bool isManager;
};

// Una unión sólo reserva espacio suficiente para el elemento más grande
union myunion_u {
    int x;
    char c;
    short s;
};

int main(int argc, char const *argv[]) {
#ifdef DEBUG
    printf("Filename: %s\nRunning program: %s\n", __FILE__, argv[0]);
#endif  // DEBUG

    struct employee_t employees[MAX_EMPLOYEES];

    union myunion_u variable_unida;
    variable_unida.x = 0x05830595;

    printf("%hx %hhx\n", variable_unida.s, variable_unida.c);

    return 0;
}