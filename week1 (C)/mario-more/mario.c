#include <cs50.h>
#include <stdio.h>

int pyramids_heigh = 0;

int main(void)
{
    do
    {
        // asking user for input
        pyramids_heigh = get_int("How tall pyramids should be? ");

        // message for informing user of invalid input
        if (pyramids_heigh <= 0 || pyramids_heigh > 8)
        {
            printf("Try a number between 1 and 8\n");
        }
    }
    while (pyramids_heigh <= 0 || pyramids_heigh > 8);

    // for loop for print all the pyramid
    for (int i = 0; i < pyramids_heigh; i++)
    {
        // print initial spaces
        for (int j = pyramids_heigh - 1; j > i; j--)
        {
            printf(" ");
        }
        // print first `#`
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        // print double space in the middle
        printf("  ");
        // print second `#`
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // New line for the next i iteration
        printf("\n");
    }
}
