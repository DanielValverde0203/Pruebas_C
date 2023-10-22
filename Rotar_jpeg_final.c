#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>


//Funcion para rotar la imagen jpeg
void jpegRotate90(const char *inputPath, const char *outputPath) {

    //Abre la imagen en modo lectura
    FILE *infile = fopen(inputPath, "rb");
    if (!infile) {
        fprintf(stderr, "Error: Can't open input file %s\n", inputPath);
        return;
    }
    //Estructura para descomprimir el archivo
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    
    //parametros de la imagen de entrada
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int num_components = cinfo.output_components;

    unsigned char *image_buffer = (unsigned char *)malloc(width * height * num_components);

    JSAMPROW row_buffer;
    //se procesan las lineas de la imagen de entrada y se almacenan en rowbuffer 
    while (cinfo.output_scanline < cinfo.output_height) {
        JDIMENSION scanline = cinfo.output_scanline;
        row_buffer = &image_buffer[(height - scanline - 1) * width * num_components];
        jpeg_read_scanlines(&cinfo, &row_buffer, 1);
    }
    //termina el proceso de descomp
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    // Abre la imagen de salida en escritura
    FILE *outfile = fopen(outputPath, "wb");
    if (!outfile) {
        fprintf(stderr, "Error: Can't create output file %s\n", outputPath);
        free(image_buffer);
        return;
    }

    //Estructura para comprimir el archivo
    struct jpeg_compress_struct cinfo_out;
    cinfo_out.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    //parametros para la imagen de salida
    cinfo_out.image_width = height;
    cinfo_out.image_height = width;
    cinfo_out.input_components = num_components;
    cinfo_out.in_color_space = cinfo.out_color_space;

    jpeg_set_defaults(&cinfo_out);
    jpeg_set_quality(&cinfo_out, 75, TRUE);

    jpeg_start_compress(&cinfo_out, TRUE);

    row_buffer = (JSAMPROW)malloc(height * num_components);

    //bucle para almacenar lineas despues de su rotacion
    while (cinfo_out.next_scanline < cinfo_out.image_height) {
        JDIMENSION scanline = cinfo_out.next_scanline;
        memcpy(row_buffer, &image_buffer[scanline * width * num_components], height * num_components);
        jpeg_write_scanlines(&cinfo_out, &row_buffer, 1);
    }
    
    //Termina el proceso de comp
    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    
    //se libera la memoria en imagebuffer y en row
    free(image_buffer);
    free(row_buffer);

    fclose(outfile);
}

int main() {
    const char *inputPath = "tiburon.jpeg";
    const char *outputPath = "rotated_tiburon.jpeg"; 
    jpegRotate90(inputPath, outputPath);
    return 0;
}
