#include <stdio.h>
#include <png.h>
#include <stdlib.h>

void convertToGrayscale(png_bytep row, int width) {
    for (int x = 0; x < width; x++) {
        png_bytep px = &(row[x * 4]); // Assuming 4 bytes per pixel (RGBA)
        int gray = (int)(0.299 * px[0] + 0.587 * px[1] + 0.114 * px[2]);
        px[0] = px[1] = px[2] = gray;
    }
}

int main() {
    FILE *pngFile = fopen("cover3.png", "rb"); // Replace "image.png" with your PNG file's path
    if (!pngFile) {
        perror("Failed to open the PNG file");
        return 1;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(pngFile);
        fprintf(stderr, "Failed to create PNG read structure\n");
        return 1;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(pngFile);
        fprintf(stderr, "Failed to create PNG info structure\n");
        return 1;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(pngFile);
        fprintf(stderr, "Error during PNG read\n");
        return 1;
    }

    png_init_io(png, pngFile);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    int color_type = png_get_color_type(png, info);

    if (color_type != PNG_COLOR_TYPE_RGBA) {
        fprintf(stderr, "The input image is not in RGBA format. Conversion not supported.\n");
        png_destroy_read_struct(&png, &info, NULL);
        fclose(pngFile);
        return 1;
    }

    png_read_update_info(png, info);

    // Create a grayscale output image
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);

    // Allocate memory for the rows
    png_bytep row = (png_bytep)malloc(4 * width);

    // Read and convert the image row by row
    for (int y = 0; y < height; y++) {
        png_read_row(png, row, NULL);
        convertToGrayscale(row, width);
        png_write_row(png, row);
    }

    // Finish writing the PNG
    png_write_end(png, NULL);

    // Clean up and close the file
    png_destroy_read_struct(&png, &info, NULL);
    fclose(pngFile);
    free(row);

    return 0;
}
