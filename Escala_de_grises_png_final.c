#include <stdio.h>
#include <stdlib.h>
#include <png.h>

//Funcion para convertir la imagen a escala de grises
void gray_scale_png(const char *img_entrada, const char *img_salida) {
	
   	//abrir archivos de entrada y salida en formato de lectura y escritura respectivamente	
	FILE *in_img = fopen(img_entrada, "rb");
    	FILE *out_img = fopen(img_salida, "wb");

	//invocar funciones para el proceso de lectura de informacion de un archivo png 
    	png_structp leer_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    	png_infop info_r = png_create_info_struct(leer_png);
    	png_init_io(leer_png, in_img);
    	png_read_info(leer_png, info_r);

	//invocar funciones para el proceso de escritura de un archivo png
    	png_structp w_png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    	png_infop w_info = png_create_info_struct(w_png);
    	png_init_io(w_png, out_img);

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

	//cierre de los archivos y liberacion de memoria de las filas guardadas en row
    	fclose(in_img);
    	fclose(out_img);
    	free(row);
	}

int main() {
   	const char *img_entrada = "pixels.png";
    	const char *img_salida = "gray_pixels.png";
    	gray_scale_png(img_entrada, img_salida);
    	return 0;
}

