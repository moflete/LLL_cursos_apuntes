#include <stdio.h>

int main(int argc, char const *argv[]) {
    int temperature;
    printf("Set temperature: \n");
    scanf("%d", &temperature);

    if (temperature >= 30) {
        printf("Cagonlaputa qué calor\n");
    } else if (temperature < 30 && temperature >= 25)
    {
        printf("Bueno, todavía es aguantable\n");
    }else{
        printf("Joder qué frío\n");
    }

    return 0;
}
