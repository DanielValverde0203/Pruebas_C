#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>

void jpegRotate90(const char *inputPath, const char *outputPath) {
    FILE *infile = fopen(inputPath, "rb");
    if (!infile) {
        fprintf(stderr, "Error: Can't open input file %s\n", inputPath);
        return;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int num_components = cinfo.output_components;

    unsigned char *image_buffer = (unsigned char *)malloc(width * height * num_components);

    JSAMPROW row_buffer;
    while (cinfo.output_scanline < cinfo.output_height) {
        JDIMENSION scanline = cinfo.output_scanline;
        row_buffer = &image_buffer[(height - scanline - 1) * width * num_components];
        jpeg_read_scanlines(&cinfo, &row_buffer, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    // Create a new JPEG file with rotated image
    FILE *outfile = fopen(outputPath, "wb");
    if (!outfile) {
        fprintf(stderr, "Error: Can't create output file %s\n", outputPath);
        free(image_buffer);
        return;
    }

    struct jpeg_compress_struct cinfo_out;
    cinfo_out.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    cinfo_out.image_width = height;
    cinfo_out.image_height = width;
    cinfo_out.input_components = num_components;
    cinfo_out.in_color_space = cinfo.out_color_space;

    jpeg_set_defaults(&cinfo_out);
    jpeg_set_quality(&cinfo_out, 75, TRUE);

    jpeg_start_compress(&cinfo_out, TRUE);

    row_buffer = (JSAMPROW)malloc(height * num_components);

    while (cinfo_out.next_scanline < cinfo_out.image_height) {
        JDIMENSION scanline = cinfo_out.next_scanline;
        memcpy(row_buffer, &image_buffer[scanline * width * num_components], height * num_components);
        jpeg_write_scanlines(&cinfo_out, &row_buffer, 1);
    }

    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);

    free(image_buffer);
    free(row_buffer);

    fclose(outfile);
}

int main() {
    const char *inputPath = "conejo.jpeg";
    const char *outputPath = "rotated_image.jpeg";  // Change the output file name here
    jpegRotate90(inputPath, outputPath);
    return 0;
}

