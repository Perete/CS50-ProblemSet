#include <stdio.h>
#include <cs50.h>

#define LENGHT 7
int random[LENGHT] = {5,6,4,3,2,9,7};
int random_sorted[LENGHT];
int low = 0;
int low_counter = 0;

void selectionSort(){
    for(int i=0; i<LENGHT; i++){
        int minIndex = i;
        for(int j=i+1; j<LENGHT; j++){
            if(random[j] > random[minIndex]){
                minIndex = j;
            }
        }

        if(minIndex != i){
            int temp = random[minIndex];
            random[minIndex] = random[i];
            random[i] = temp;
        }

    }
}

int main(void){

    selectionSort();

}
