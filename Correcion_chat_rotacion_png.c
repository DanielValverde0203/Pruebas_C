#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

// Función para rotar la imagen
void rotate_image_png(const char *img_entrada, const char *img_salida) {
    // Abrir archivos de entrada y salida en modo de lectura y escritura, respectivamente
    FILE *in_img = fopen(img_entrada, "rb");
    FILE *out_img = fopen(img_salida, "wb");

    // Invocar funciones para leer información del archivo PNG
    png_structp leer_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_r = png_create_info_struct(leer_png);
    png_init_io(leer_png, in_img);
    png_read_info(leer_png, info_r);

    // Invocar funciones para escribir un archivo PNG
    png_structp w_png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop w_info = png_create_info_struct(w_png);
    png_init_io(w_png, out_img);

    // Parámetros de los archivos de entrada y salida
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_type, filter_method;
    png_get_IHDR(leer_png, info_r, &width, &height, &bit_depth, &color_type, &interlace_method, compression_type, &filter_method);
    png_set_IHDR(w_png, w_info, width, height, bit_depth, color_type, interlace_method, compression_type, filter_method);
    png_write_info(w_png, w_info);

    // Asignar memoria para los datos de la imagen
    png_bytepp row_pointers_original = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++) {
        row_pointers_original[i] = (png_byte*)malloc(png_get_rowbytes(leer_png, info_r));
    }
    png_read_image(leer_png, row_pointers_original);

    // Asignar memoria para los datos de la imagen rotada
    png_bytepp row_pointers_new = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++) {
        row_pointers_new[i] = (png_byte*)malloc(png_get_rowbytes(w_png, w_info));
    }

    // Realizar la rotación de la imagen
    for (int i = 0; i < height; i++) { // Filas
        for (int j = 0; j < width / 2; j++) { // Columnas
            // Intercambiar los valores de píxeles para rotar la imagen
            png_bytep temp = row_pointers_original[i][j];
            row_pointers_new[i][j] = row_pointers_original[i][width - j - 1];
            row_pointers_new[i][width - j - 1] = temp;
        }
    }

    // Escribir la imagen rotada
    png_write_image(w_png, row_pointers_new);

    // Liberar recursos y finalizar la escritura
    png_write_end(w_png, w_info);
    png_destroy_read_struct(&leer_png, &info_r, (png_infopp)NULL);
    png_destroy_write_struct(&w_png, &w_info);

    // Cerrar archivos y liberar la memoria asignada para los datos de las filas
    fclose(in_img);
    fclose(out_img);
    for (int i = 0; i < height; i++) {
        free(row_pointers_original[i]);
        free(row_pointers_new[i]);
    }
    free(row_pointers_original);
    free(row_pointers_new);
}

int main() {
    const char *img_entrada = "pixels.png";
    const char *img_salida = "pixels_rot.png";

    rotate_image_png(img_entrada, img_salida);

    return 0;
}
