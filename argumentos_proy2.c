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
	printf("rotate  recibe angulo de rotación de la imagen de entrada, la imagen resultante es la misma pero girada ese ángulo\n");
        printf("otro    coming soon\n");
	printf("\n");
       	printf("Ejemplos:\n");
        printf("\n");
	printf("modif_image.exe -i imagen_entrada -o imagen_salida rotate 90\n");
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
	
	for (int i=1; i < argc; i++) {

		// Este argumento se debe poner solo ya que solamente imprime el mensaje de ayuda y termina el programa.
		if (strcmp(argv[i], "-h") == 0) {
			ayuda();
			return 0;
		}

		// Verifica que se incluya el argumento -i.
		else if (strcmp(argv[i], "-i") == 0) {
			
			printf("Se guarda el tercer argumento como el nombre de la imagen.\n");
			printf("Path img original: %s\n", argv[2]);
			
			char *formato = checkear(argv[2]);
			printf("Formato: %s\n", formato);

		}

		// Verifica que se incluya el argumento -o.
		else if (strcmp(argv[i], "-o") == 0) {

			printf("Se guarda el quinto argumento en una variable para usarlo como nombre del archivo resultante.\n");
			printf("Path img resultante: %s\n", argv[4]);
		}
	}

	return 0;
}
