#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <png.h>

//Funcion para rotar la imagen
void rotateImagePng(const char *img_entrada, const char *img_salida) {
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
    	png_set_IHDR(w_png, w_info, width, height, bit_depth, color_type, interlace_method, compression_type, filter_method);
      png_write_info(w_png, w_info);


      png_bytepp* row_pointers_original = (png_bytep*)malloc(sizeof(png_bytep) * height);
      for (int i = 0; i < height; i++) {
        row_pointers_original[i] = (png_byte*)malloc(png_get_rowbytes(leer_png, info_r));
      }
      png_read_image(leer_png, row_pointers_original);
      
  
      png_bytepp** row_pointers_new = row_pointers_original;
  
      // Iterar sobre los píxeles de la imagen original
      for (int i = 0; i < height; i++) { //rows
        for (int j = 0; j < width/2; j++) { //cols

            row_pointers_new[i][j] = row_pointers_original[i][width - j-1];
            row_pointers_new[i][width - j-1] = row_pointers_original[i][j];
  
        }
      }

      // Iterar sobre los píxeles de la imagen original
      for (int i = 0; i < height/2; i++) { //rows
        for (int j = 0; j < width; j++) { //cols
            png_bytep** d = row_pointers_new[i][j];
            row_pointers_new[i][j] = row_pointers_new[height - i-1][j];
            row_pointers_new[height - i-1][j] = d;
        }
      }
      

      png_write_image(w_png, row_pointers_new);

      //fin de la escritura del archivo y libera los procesos de lectura y escritura 
      png_write_end(w_png, w_info);
      png_destroy_read_struct(&leer_png, &info_r, (png_infopp)NULL);
      png_destroy_write_struct(&w_png, &w_info);

	    //cierre de los archivos y liberacion de memoria de las filas guardadas en row
    	fclose(in_img);
    	fclose(out_img);
	}


int main(int argc, char *argv[]) {
  /*
  if (argc != 3) {
    fprintf(stderr, "Uso: %s <archivo de entrada> <ángulo de rotación>\n", argv[0]);
    return 1;
  }
  */
  rotateImagePng("/home/prichavarria/Downloads/lapiz.png", "/home/prichavarria/Downloads/lapiz-ROTATED.png");
  
  return 0;
}
