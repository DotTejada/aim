#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char *argv[]) {
    char brightness_table[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    int width, height, channels;
    unsigned char *data = stbi_load("check.png", &width, &height, &channels, STBI_grey);
    if (!data) { exit(1); }

    int size = width * height * STBI_grey;
    printf("width: %d, height: %d, channels: %d\n", width, height, STBI_grey);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", brightness_table[data[i * width + j]*9/255]);
        }
        printf("\n");
    }

    for (int a = 0; a < 150; a++) {
        printf("#");
    }
    printf("\n");
    stbi_image_free(data);
    return 0;
}
