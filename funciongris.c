#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Funcion que convierte la imagen a escala de gris
void Grayscale(uint8_t* image, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        // calculo de escala de gris para pixeles
        uint8_t r = image[3 * i];
        uint8_t g = image[3 * i + 1];
        uint8_t b = image[3 * i + 2];
        uint8_t gray = (uint8_t)(0.2989 * r + 0.5870 * g + 0.1140 * b);

        // conversion de los pixeles a gris
        image[3 * i] = gray;
        image[3 * i + 1] = gray;
        image[3 * i + 2] = gray;
    }
}

int main() {
    int width = 640; // Width of the image
    int height = 480; // Height of the image
    uint8_t* image = (uint8_t)malloc(width height * 3); // Assuming 3 channels (RGB)

    // Fill 'image' with your image data

    // Call the function to convert to grayscale
    Grayscale(image, width, height);

    // Now 'image' contains the grayscale version of the original image

    free(image);

    return 0;
}
