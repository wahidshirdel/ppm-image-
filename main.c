#include <stdio.h>
#include <stdlib.h>
typedef struct {
   unsigned char r, g, b;
} Pixel;
int main() {

    FILE *inputFile, *outputFile;
    char format[3];
    int width, height, maxColorValue;   

   // Open the input PPM file in binary mode

     inputFile = fopen("colorful_image.ppm", "rb");

    if (inputFile == NULL) {
      printf("Error: Unable to open input file!\n");
      return 1;
    }
    

    // Read PPM header (P6 format)

    fscanf(inputFile, "%s", format);
    if (format[0] != 'P' || format[1] != '6') {
      printf("Error: Unsupported PPM format! Only binary PPM (P6) is supported.\n");
      fclose(inputFile);
      return 1;
      }



    // Skip comments in the header

    char c = fgetc(inputFile);
    while (c == '#') {
     while (fgetc(inputFile) != '\n');
        c = fgetc(inputFile);
    }
    ungetc(c, inputFile);   
   // Read image dimensions and max color value
   fscanf(inputFile, "%d %d %d", &width, &height, &maxColorValue);
   fgetc(inputFile);  // Read the newline after the max color value
    

    // Allocate memory for the pixel data

     Pixel *image = (Pixel *)malloc(width * height * sizeof(Pixel));
    if (image == NULL) {
        printf("Error: Memory allocation failed!\n");
        fclose(inputFile);
        return 1;
    }


    // Read pixel data from P6 file

    fread(image, sizeof(Pixel), width * height, inputFile);

    fclose(inputFile);


    // Open the output PGM file (P2 format)

    outputFile = fopen("output2.ppm", "w");
    if (outputFile == NULL) {
        printf("Error: Unable to open output file!\n");
        free(image);
       return 1;
       }

    // Write P2 header

    fprintf(outputFile, "P2\n%d %d\n%d\n", width, height, maxColorValue);  

   // Convert each RGB pixel to grayscale and write the data to the P2 file

    for (int i = 0; i < width * height; i++) {
        unsigned char r = image[i].r;
        unsigned char g = image[i].g;
        unsigned char b = image[i].b;
        unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
        fprintf(outputFile, "%d ", gray);       

       // Optionally, add a newline after every row of pixels for formatting        if ((i + 1) % width == 0) {

            fprintf(outputFile, "\n");
        }
   }

    // Cleanup
    fclose(outputFile);
    free(image);    
    printf("Conversion to grayscale (P2) completed successfully.\n");
   return 0;
}
