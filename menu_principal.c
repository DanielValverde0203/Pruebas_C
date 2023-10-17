#include <stdio.h>
#include <png.h>
#include <jpeglib.h>
#include <string.h>

void menu_principal(char img_name[]) {
	
	//clasificacion del formato del archivo
	char *format = NULL;
	char *token = strtok(img_name, ".");

	while (token != NULL){
		format = token;
		token = strtok(NULL, ".");
	}

	//verificar la manipulacion sobre la imagen
	int opcion;
	printf("el formato es: %s\n", format);
	printf("seleccione 1 para rotar o 2 para escala de grises: ");
	scanf("%d", &opcion);

	switch (opcion){
		case 1:
			if (strcmp(format, "jpeg") == 0){
				//llamar_funcion_rotate_jpeg()
			}
			else if (strcmp(format, "png") == 0){
				//llamar_funcion_rotate_png()
			}

		case 2:
			if (strcmp(format, "jpeg") == 0){
                                //llamar_funcion_gray_scale_jpeg()
                        }
                        else if (strcmp(format, "png") == 0){
                                //llamar_funcion_gray_scale_png()
                        }
					
	}
}

int main(){
	char img_name[] = "img.jpeg";
	menu_principal(img_name);
	return 0;
}

