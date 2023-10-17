#include <stdio.h>
#include <jpeglib.h>

// Function to convert a color pixel to grayscale
unsigned char rgbToGrayscale(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
}

void jpegToGrayscale(int width, int height, unsigned char imageData[width * height * 3], const char *outputFileName) {
    struct jpeg_compress_struct cinfo;
    FILE *outfile;

    cinfo.err = jpeg_std_error(&cinfo.err);
    jpeg_create_compress(&cinfo);

    outfile = fopen(outputFileName, "wb");
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 1;
    cinfo.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo);

    JSAMPLE row_buffer[width];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char r = imageData[(y * width + x) * 3];
            unsigned char g = imageData[(y * width + x) * 3 + 1];
            unsigned char b = imageData[(y * width + x) * 3 + 2];

            row_buffer[x] = rgbToGrayscale(r, g, b);
        }
        jpeg_write_scanlines(&cinfo, &row_buffer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(outfile);
}

int main() {
    int width = 800; // largo imagen cambiar
    int height = 600; // alto imagen cambiar
    const char *outputFileName = "output.jpg";

    // informacion real de la imagen
    unsigned char imageData[width * height * 3];

    // llama a la funcion con los aprametros de la imagen
    jpegToGrayscale(width, height, imageData, outputFileName);

    return 0;
}
