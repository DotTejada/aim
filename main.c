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

    int color_channels;
    if (strcmp(color_flag, "-g") == 0) {
        color_channels = STBI_grey_alpha;
    } else if (strcmp(color_flag, "-c") == 0) {
        color_channels = STBI_rgb_alpha;
    }
    unsigned char *data = stbi_load(filename, &width, &height, &channels, color_channels);
    if (!data) { 
        printf("Couldn't load file data\n"); 
        exit(1); 
    }

    int scaled_height = ((float)height / (float)width) * (float)scaled_width / 2;

    unsigned char *data_scaled = stbir_resize_uint8_srgb(data,  width,  height,  0,
                                                         0, scaled_width, scaled_height, 0,
                                                         color_channels);
    if (!data_scaled) {
        printf("Couldn't resize data\n"); 
        exit(1);
    }

    if (color_channels == STBI_grey_alpha) {
        for (int i = 0; i < scaled_height; i++) {
            for (int j = 0; j < scaled_width * color_channels; j += color_channels) {

                int offset = i * scaled_width * color_channels + j;
                float grey = data_scaled[offset];
                float a = data_scaled[offset + 1];

                float brightness = grey * a/255.0;
                printf("%c", brightness_table[(int)(brightness * 9.0/255.0)]);
            }
            printf("\n");
        }
    } else if (color_channels == STBI_rgb_alpha) {
        // acquire r, g, b from input
        // get brightness from
        // Y = 0.299R + 0.587G + 0.114B
        // make sure to take alpha into account
        // apply color with
        // 16 + 36 × r + 6 × g + b (0 ≤ r, g, b ≤ 5)
        // print with
        // \x1b[38;5;{ID}m{Char}\x1b[0m
        for (int i = 0; i < scaled_height; i++) {
            for (int j = 0; j < scaled_width * color_channels; j += color_channels) {

                int offset = i * scaled_width * color_channels + j;
                float r = data_scaled[offset];
                float g = data_scaled[offset + 1];
                float b = data_scaled[offset + 2];
                float a = data_scaled[offset + 3];

                float brightness = (0.299 * r + 0.587 * g + 0.114 * b) * a/255.0;
                char pixel = brightness_table[(int)(brightness * 9.0/255.0)];
                r *= 5.0/255.0;
                g *= 5.0/255.0;
                b *= 5.0/255.0;
                int pixel_color = 16 + 36 * (int)r + 6 * (int)g + (int)b;
                printf("\x1b[38;5;%dm%c\x1b[0m", pixel_color, pixel);
            }
            printf("\n");
        }
    } else {
    }

    stbi_image_free(data_scaled);
    stbi_image_free(data);
    return 0;
}
