#include <stdio.h>
#include <png.h>
#include <jpeglib.h>
#include <zlib.h>
#include <string.h>

void ayuda () {
	printf("El programa realiza transformaciones a imagenes.\n");
        printf("\n");
        printf("Opciones:\n");
        printf("\n");
        printf("-i      recibe path de imagen a modificar\n");
        printf("-o      recibe path de imagen resultante\n");
        printf("-h      solamente imprime la lista de opciones del programa\n");
        printf("        no acepta más argumentos\n");
        printf("\n");
	printf("Algoritmos (van después de indicar el nombre del archivo de entrada y el de salida):\n");
	printf("\n");
	printf("rotate  gira la imagen 90 grados\n");
        printf("grayscl	convierte la imagen a la escala de grises\n");
	printf("\n");
       	printf("Ejemplos:\n");
        printf("\n");
	printf("modif_image.exe -i imagen_entrada -o imagen_salida rotate\n");
	printf("Nota: se debe respetar el órden de argumentos presente en el ejemplo anterior.\n");
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
			
			printf("Path img original: %s\n", argv[2]);
			
			image_in = fopen(argv[2],"r");

			formato = checkear(argv[2]);

			printf("Formato: %s\n", formato);

		}

		// Verifica que se incluya el argumento -o.
		else if (strcmp(argv[i], "-o") == 0) {
			
			image_out = fopen(argv[4], "w+");
			
			printf("Path img resultante: %s\n", argv[4]);
		}
	}
	
	if (strcmp(argv[5], "rotate") == 0) {
	
		if (strcmp(formato, "jpeg") == 0){
                        //llamar_funcion_rotate_jpeg()
			printf("Rotar jpeg\n");
                }
                else if (strcmp(formato, "png") == 0){
                        //llamar_funcion_rotate_png()
                	printf("Rotar png\n");
		}
	}

	else if (strcmp(argv[5], "gray_scale") == 0) {
	
		if (strcmp(formato, "jpeg") == 0){
                        //llamar_funcion_gray_scale_jpeg()
                	printf("Gris jpeg\n");
		}
                else if (strcmp(formato, "png") == 0){
                        //llamar_funcion_gray_scale_png()
                	printf("Gris png\n");
		}
	}

	fclose(image_in);
	fclose(image_out);

	return 0;
}
