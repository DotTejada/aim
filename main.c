#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        exit(1);
    }
    char* filename = argv[1];
    int scaled_width = atoi(argv[2]);
    char* color_flag = argv[3];

    char brightness_table[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    int width, height, channels;

    int color;
    if (strcmp(color_flag, "-g") == 0) {
        color = STBI_grey;
    } else {
        color = STBI_rgb;
    }
    unsigned char *data = stbi_load(filename, &width, &height, &channels, color);
    if (!data) { 
        printf("Couldn't load file data\n"); 
        exit(1); 
    }
    //printf("width: %d, height: %d, channels: %d\n", width, height, channels);

    int scaled_height = ((float)height / (float)width) * (float)scaled_width / 2;
    //printf("width: %d, height: %d, channels: %d\n", s_width, s_height, s_channel);

    unsigned char *data_scaled = stbir_resize_uint8_srgb(data,  width,  height,  0,
                                                         0, scaled_width, scaled_height, 0,
                                                         color);
    if (!data_scaled) { exit(1); }
    //printf("width: %d, height: %d, channels: %d\n", s_width, s_height, s_channel);

    if (color == STBI_grey) {
        for (int i = 0; i < scaled_height; i++) {
            for (int j = 0; j < scaled_width; j++) {
                printf("%c", brightness_table[(int)(data_scaled[i * scaled_width + j]*9.0/255.0)]);
            }
            printf("\n");
        }
    } else {
        // acquire r, g, b from input
        // get brightness from
        // Y = 0.299R + 0.587G + 0.114B
        // apply color with
        // 16 + 36 × r + 6 × g + b (0 ≤ r, g, b ≤ 5)
        // print with
        // \x1b[38;5;{ID}m{Char}\x1b[0m
        for (int i = 0; i < scaled_height; i++) {
            for (int j = 0; j < scaled_width * color; j += 3) {

                int value = i * scaled_width * color + j;
                float r = data_scaled[value];
                float g = data_scaled[value + 1];
                float b = data_scaled[value + 2];

                float brightness = 0.299 * r + 0.587 * g + 0.114 * b;
                char pixel = brightness_table[(int)(brightness * 9.0/255.0)];
                r *= 5.0/255.0;
                g *= 5.0/255.0;
                b *= 5.0/255.0;
                int pixel_color = 16 + 36 * (int)r + 6 * (int)g + (int)b;
                printf("\x1b[38;5;%dm%c\x1b[0m", pixel_color, pixel);
            }
            printf("\n");
        }
    }

    stbi_image_free(data_scaled);
    stbi_image_free(data);
    return 0;
}
