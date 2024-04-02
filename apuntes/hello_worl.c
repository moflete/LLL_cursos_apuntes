#include <stdint.h>
#include <stdio.h>

struct header_t {
    uint16_t id;
    uint16_t version;
    uint16_t count;
};

void header_init(struct header_t self){
    self.id = 0xFFAF;
    self.version = 0x0001;
    self.count = 0;
}

int main(int argc, char **argv) {
    printf("Hello World!!\n");

    struct header_t app_data;
    header_init(app_data);

    printf("App data initialize as\nid: %d\nversion: %d\ncount: %d\n", app_data.id, app_data.version, app_data.count);


    return 0;
}