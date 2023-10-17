#include <stdio.h>
#include <png.h>
#include <jpeglib.h>
#include <string.h>

void menu_principal (char img_name[]){
	//clasificacion del formato del archivo
	char *format = NULL;
	char *token = strtok(img_name, ".");

	while (token != NULL){
		format = token;
		token = strtok(NULL, ".");
	}
}

int main(){
	char img_name[] = "img.png";
	return 0;
}







