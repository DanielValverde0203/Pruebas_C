#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <jpeglib.h>
#include <zlib.h>
#include <string.h>

#include "ayuda.h"

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

void gray_scale_png(FILE *imagen_ent, FILE *imagen_salid) {

        //invocar funciones para el proceso de lectura de informacion de un archivo png 
        png_structp leer_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info_r = png_create_info_struct(leer_png);
        png_init_io(leer_png, imagen_ent);
        png_read_info(leer_png, info_r);

        //invocar funciones para el proceso de escritura de un archivo png
        png_structp w_png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop w_info = png_create_info_struct(w_png);
        png_init_io(w_png, imagen_salid);

        // parametros de los archivos de lectura y escritura
        png_uint_32 width, height;
        int bit_depth, color_type, interlace_method, compression_type, filter_method;
        png_get_IHDR(leer_png, info_r, &width, &height, &bit_depth, &color_type, &interlace_method, &compression_type, &filter_method);
        png_set_IHDR(w_png, w_info, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(w_png, w_info);
        png_bytep row = (png_bytep)malloc(png_get_rowbytes(leer_png, info_r));

        //recorrer la imagen para cambiar las propiedades de cada pixel por las filas del archivo png
        for (int i = 0; i < height; i++) {
                png_read_row(leer_png, row, NULL);
                for (int j = 0; j < width; j++) {
                png_bytep px = &(row[j]);
                int gray = (int)(0.299 * px[0] + 0.587 * px[0] + 0.114 * px[0]);
                px[0] = gray;
                }
                png_write_row(w_png, row);
        }

        //fin de la escritura del archivo y libera los procesos de lectura y escritura 
        png_write_end(w_png, w_info);
        png_destroy_read_struct(&leer_png, &info_r, (png_infopp)NULL);
        png_destroy_write_struct(&w_png, &w_info);

        //liberacion de memoria de las filas guardadas en row
        free(row);
}

void rotate_jpeg(FILE *imagen_ent, FILE *imagen_salid) {

    //Estructura para descomprimir el archivo
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, imagen_ent);

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

    //Estructura para comprimir el archivo
    struct jpeg_compress_struct cinfo_out;
    cinfo_out.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, imagen_salid);

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
}

char *checkear (char *img_name) {
	
	char *format = NULL;
        char *token = strtok(img_name, ".");

        while (token != NULL){
                format = token;
                token = strtok(NULL, ".");
        }

	return format;
}

int main (int argc, char *argv[]) {
	if (argc == 1) {
		printf("Ingrese más argumentos\n");
		return 1;
	}

	FILE *image_in;
	FILE *image_out;
	char *formato;

	for (int i=1; i < argc; i++) {

		// Este argumento se debe poner solo ya que solamente imprime el mensaje de ayuda y termina el programa.
		if (strcmp(argv[i], "-h") == 0) {
			ayuda();
			return 0;
		}

		// Verifica que se incluya el argumento -i.
		else if (strcmp(argv[i], "-i") == 0) {
			
			image_in = fopen(argv[2],"r");

			formato = checkear(argv[2]);

		}

		// Verifica que se incluya el argumento -o.
		else if (strcmp(argv[i], "-o") == 0) {
			
			image_out = fopen(argv[4], "w+");
		}
	}
	
	if (strcmp(argv[5], "rotate") == 0) {
	
		if (strcmp(formato, "jpeg") == 0){
                        
			rotate_jpeg(image_in, image_out);
                }
                else if (strcmp(formato, "png") == 0){
                        //llamar_funcion_rotate_png()
                	printf("Rotar png\n");
		}
	}

	else if (strcmp(argv[5], "gray_scale") == 0) {
	
		if (strcmp(formato, "jpeg") == 0){

			gray_scale_jpeg(image_in, image_out);
		}
                else if (strcmp(formato, "png") == 0){
                        
			gray_scale_png(image_in, image_out);
		}
	}

	fclose(image_in);
	fclose(image_out);

	return 0;
}
