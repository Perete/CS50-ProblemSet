#include <stdio.h>
#include <cs50.h>

int main(void){

    // char* s = get_string("s: ");
    int num = 50;
    int* numAdd = &num;

    // printf("%c\n",*s);
    printf("%p\n",*numAdd);
}
