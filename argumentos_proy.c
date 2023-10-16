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
        printf("rotate  recibe angulo de rotación de la imagen\n");
      	printf("otro    coming soon\n");
        printf("-h      solamente imprime la lista de opciones del programa\n");
        printf("        no acepta más argumentos\n");
        printf("\n");
       	printf("Ejemplos:\n");
        printf("\n");
	printf("modif_image.exe -i imagen_entr -o imagen_salid rotate 90\n");
	printf("Nota: se debe respetar el órden de argumentos presente en el ejemplo anterior.\n");
}



int main(int argc, char *argv[]) {
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

		else if (strcmp(argv[i], "-i") == 0) {
			printf("Se guarda el tercer argumento como el nombre de la imagen.\n");
			printf("Path img original: %s\n", argv[2]);
		}

		else if (strcmp(argv[i], "-o") == 0) {
			printf("Se guarda el quinto argumento en una variable para usarlo como nombre del archivo resultante.\n");
			printf("Path img resultante: %s\n", argv[4]);
		}
	}
	return 0;
}
