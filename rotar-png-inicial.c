#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/png.h"
#include <math.h>



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
    	png_set_IHDR(w_png, w_info, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    	png_write_info(w_png, w_info);

      png_bytepp row_pointers_original = png_get_rows(leer_png, info_r);

      png_bytepp row_pointers_new = (png_bytepp)png_malloc(w_png, sizeof(png_bytepp) * height);
      for (int i = 0; i < height; i++) {
        row_pointers_new[i] = (png_bytep)png_malloc(w_png, width);
      }


      // Convertir el ángulo a radianes
      int angle = 90;
      float angle_rad = angle * M_PI / 180.0f;

      // Crear una matriz de rotación
      float rotation_matrix[2][2] = {
        {cos(angle_rad), -sin(angle_rad)},
        {sin(angle_rad), cos(angle_rad)}
      };

      // Iterar sobre los píxeles de la imagen original
      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          // Calcular la posición del píxel rotado
          int x = i * rotation_matrix[0][0] + j * rotation_matrix[0][1];
          int y = i * rotation_matrix[1][0] + j * rotation_matrix[1][1];

          // Copiar el píxel a la imagen rotada
          if (x >= 0 && x < width && y >= 0 && y < height) {
            row_pointers_new[y][x] = row_pointers_original[j][i];
          }
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

  rotateImagePng("/home/prichavarria/Downloads/pinkpinkpink.png", "/home/prichavarria/Downloads/pinkpinkpink-ROTATED.png");
  
}
  
