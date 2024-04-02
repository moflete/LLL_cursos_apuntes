#include <stdio.h>
#define DEBUG 1

int main(int argc, char const *argv[]) {
#ifdef DEBUG
    printf("Filename: %s\nRunning program: %s\n", __FILE__, argv[0]);
#endif  // DEBUG

    for (size_t i = 0; i < 50; i++) {
        printf("%d\n", (int)i);
    }
    return 0;
}
