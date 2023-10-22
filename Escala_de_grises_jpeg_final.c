#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

// Funcion para convertir la imagen jpeg a escala de grises
void jpegToGrayscale(const char* inputPath, const char* outputPath) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    //Abre la imagen de entrada en lectura
    FILE* infile = fopen(inputPath, "rb");
    if (!infile) {
        fprintf(stderr, "Error opening input file\n");
        return;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);

    // configuracion para el color de salida
    cinfo.out_color_space = JCS_GRAYSCALE;
    jpeg_start_decompress(&cinfo);

    struct jpeg_compress_struct cinfo_out; 
    struct jpeg_error_mgr jerr_out;


    // Abre la imagen de salida en escritura
    FILE* outfile = fopen(outputPath, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file\n");
        return;
    }

    //estructuras para comprimir el archivo
    cinfo_out.err = jpeg_std_error(&jerr_out);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    //Parametros para la nueva imagen de salida
    cinfo_out.image_width = cinfo.output_width;
    cinfo_out.image_height = cinfo.output_height;
    cinfo_out.input_components = 1;
    cinfo_out.in_color_space = JCS_GRAYSCALE;
    
    jpeg_set_defaults(&cinfo_out);
    jpeg_set_quality(&cinfo_out, 75, TRUE);
    jpeg_start_compress(&cinfo_out, TRUE);

    //Almacena los pixeles de la nueva imagen
    JSAMPROW row_buffer[1];
    row_buffer[0] = (JSAMPROW)malloc(cinfo_out.image_width);

    //Bucle encargado de hacer la conversion a escala de grises
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_buffer, 1);
        jpeg_write_scanlines(&cinfo_out, row_buffer, 1);
    }

    //El archivo se termina de escribir y se liberan los procesos de compress y decompress
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);

    //cierra ambos archivos y se libera la memoria de las filas guardadas en row buffer
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
