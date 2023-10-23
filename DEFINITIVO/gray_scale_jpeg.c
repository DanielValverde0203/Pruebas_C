#include "gray_scale_jpeg.h"

void gray_scale_jpeg(FILE *imagen_ent, FILE *imagen_salid) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    //estructuras para descomprimir el archivo
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, imagen_ent);
    jpeg_read_header(&cinfo, TRUE);

    // configuracion para el color de salida
    cinfo.out_color_space = JCS_GRAYSCALE;
    jpeg_start_decompress(&cinfo);

    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr_out;

    //estructuras para comprimir el archivo
    cinfo_out.err = jpeg_std_error(&jerr_out);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, imagen_salid);

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

    //se libera la memoria de las filas guardadas en row buffer
    free(row_buffer[0]);
}
