#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char *argv[]) {
    int width, height, channels;
    unsigned char *data = stbi_load("check.png", &width, &height, &channels, STBI_rgb_alpha);
    if (!data) { exit(1); }
    int size = width * height * channels;
    printf("width: %d, height: %d, channels: %d\n", width, height, channels);
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
        if (i != 0 && (i + 1) % (width * channels) == 0) {
            printf("\n");
        }
    }
    printf("\n");
    stbi_image_free(data);
    return 0;
}
