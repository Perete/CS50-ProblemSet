#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    if(argc<2){
        printf("Usage: ./recover [infile]\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *inptr = fopen(infile,"r");

    if(inptr == NULL){
        printf("Could not open %s file\n", infile);
        return 1;
    }

    uint8_t buffer[512];
    FILE *outptr = NULL;
    // char outputFile[15];
    char outputFile[8];
    int jpegFound = 0;
    int fileCounter = 0;

    // while(n!=10){
    //     printf("%d\n", n);
    //     n++;
    // }

    while(fread(&buffer, 512, 1, inptr) != 0){
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] >> 4) == 0b1110){
            printf("Cabecera JPG detectada: 0x%x 0x%x 0x%x 0x%x\n",buffer[0],buffer[1],buffer[2],buffer[3]);

            if(jpegFound){
                fclose(outptr);
            }

            jpegFound = 1;
            // sprintf(outputFile, "output/%03i.jpg", fileCounter);
            sprintf(outputFile, "%03i.jpg", fileCounter);
            outptr = fopen(outputFile, "w");

            if(outptr == NULL){
                printf("Could not create output JPEG file.\n");
                return 1;
            }

            fileCounter++;


        }

        if(jpegFound){
            fwrite(buffer, 512, 1, outptr);
        }
    }

    if(outptr != NULL){
        fclose(outptr);
    }

    fclose(inptr);
    return 0;
}

