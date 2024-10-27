#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void){
    string strings[] = {"hola", "adios", "gato", "perro"};

    string s = get_string("String:");

    for(int i=0; i<3; i++){
        printf("strings: %s\n", strings[i]);
        if(strcmp(strings[i],s) == 0){
            printf("Found");
            return 0;
        }else{
            printf("Not found\n");
        }
    }

    return 1;
}
