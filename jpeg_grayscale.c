#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

void jpegToGrayscale(const char* inputPath, const char* outputPath) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE* infile = fopen(inputPath, "rb");
    if (!infile) {
        fprintf(stderr, "Error opening input file\n");
        return;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);

    cinfo.out_color_space = JCS_GRAYSCALE;
    jpeg_start_decompress(&cinfo);

    FILE* outfile = fopen(outputPath, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file\n");
        return;
    }

    JSAMPROW row_buffer[1];
    row_buffer[0] = (JSAMPROW)malloc(cinfo.output_width * cinfo.output_components);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_buffer, 1);
        fwrite(row_buffer[0], 1, cinfo.output_width, outfile);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    fclose(outfile);
}

int main() {
    const char* inputPath = "mike.jpeg";
    const char* outputPath = "mike_gris.jpeg";
    jpegToGrayscale(inputPath, outputPath);
    return 0;
}