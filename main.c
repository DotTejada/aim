#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        exit(1);
    }
    char brightness_table[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    int width, height, channels;

    unsigned char *data = stbi_load(argv[1], &width, &height, &channels, STBI_grey);
    if (!data) { 
        printf("Couldn't load file data"); 
        exit(1); 
    }
    //printf("width: %d, height: %d, channels: %d\n", width, height, channels);

    int s_width = atoi(argv[2]); 
    int s_height = ((float)height / (float)width) * (float)s_width / 2;
    int s_channel = STBI_grey;
    //printf("width: %d, height: %d, channels: %d\n", s_width, s_height, s_channel);

    unsigned char *data_scaled = stbir_resize_uint8_srgb(data,  width,  height,  0,
                                                         0, s_width, s_height, 0,
                                                         s_channel);
    if (!data_scaled) { exit(1); }
    //printf("width: %d, height: %d, channels: %d\n", s_width, s_height, s_channel);

    for (int i = 0; i < s_height; i++) {
        for (int j = 0; j < s_width; j++) {
            printf("%c", brightness_table[data_scaled[i * s_width + j]*9/255]);
        }
        printf("\n");
    }

    stbi_image_free(data_scaled);
    stbi_image_free(data);
    return 0;
}
