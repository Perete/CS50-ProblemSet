#include <stdio.h>


int main(void){
    int myVariable = 15;

    int* Addres = &myVariable;

    *Addres = 25;
}
