#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int HEADER_SIZE = 44;
uint8_t* header[HEADER_SIZE];

int numbers[];

int main(void){

    // Open files and determine scaling factor
    FILE *input = fopen("file.txt", "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    const int stringSize = 5;
    uint8_t b[stringSize];
    printf("%lu\n", sizeof(b));
    printf("%lu\n", sizeof(b[0]));

    fread(&b, stringSize, 1, input);

    for(int i=0; i<stringSize; i++){
        // fread(&b[i], sizeof(b[i]), 1, input);
        printf("[%d]%c\n", i, b[i]);
        // printf("Double number:[%d]\n", doubleNumbers[i]);
    }

    fread(&b, stringSize, 1, input);

    for(int i=0; i<stringSize; i++){
        // fread(&b[i], sizeof(b[i]), 1, input);
        printf("[%d]%c\n", i, b[i]);
    }
    // while (fread(&b, sizeof(b), 1, input) !=0)
    // {
    //     // fwrite(&b, sizeof(b), 1, dst);
    //     printf("%c\n", b);
    // }

    fclose(input);

}
