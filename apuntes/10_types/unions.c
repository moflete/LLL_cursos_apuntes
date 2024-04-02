#include <stdio.h>

typedef union {
    unsigned int addr;
    unsigned char bytes[4];
} ipv4_addr;

union HWRegister {
    struct {
        unsigned char control;
        unsigned char direction;
        unsigned char parity;
        unsigned char stopbits;
    };
    unsigned int reg;
};

int main(int argc, char const *argv[]) {
    ipv4_addr my_addr = {0};

    my_addr.bytes[0] = 127;
    my_addr.bytes[1] = 0;
    my_addr.bytes[2] = 0;
    my_addr.bytes[3] = 1;

    printf("%x\n", my_addr.addr);

    union HWRegister gpioa;

    gpioa.reg = 0x41424344;

    printf("%x\n", gpioa.stopbits);
    printf("%x\n", gpioa.parity);

    return 0;
}
