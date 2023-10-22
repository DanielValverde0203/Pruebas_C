#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

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

    // Ensure the output color space is set to grayscale
    cinfo.out_color_space = JCS_GRAYSCALE;
    jpeg_start_decompress(&cinfo);

    struct jpeg_compress_struct cinfo_out; // Create a new compression structure
    struct jpeg_error_mgr jerr_out;

    FILE* outfile = fopen(outputPath, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file\n");
        return;
    }

    cinfo_out.err = jpeg_std_error(&jerr_out);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    // Set the output image parameters
    cinfo_out.image_width = cinfo.output_width;
    cinfo_out.image_height = cinfo.output_height;
    cinfo_out.input_components = 1; // Grayscale image has only one component
    cinfo_out.in_color_space = JCS_GRAYSCALE;
    
    jpeg_set_defaults(&cinfo_out);
    jpeg_set_quality(&cinfo_out, 75, TRUE);
    jpeg_start_compress(&cinfo_out, TRUE);

    JSAMPROW row_buffer[1];
    row_buffer[0] = (JSAMPROW)malloc(cinfo_out.image_width);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_buffer, 1);
        jpeg_write_scanlines(&cinfo_out, row_buffer, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    fclose(infile);
    fclose(outfile);
    free(row_buffer[0]);
}

int main() {
    const char* inputPath = "conejo.jpeg";
    const char* outputPath = "output_gray.jpeg";
    jpegToGrayscale(inputPath, outputPath);
    return 0;
}
