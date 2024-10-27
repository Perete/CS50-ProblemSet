#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int accumOdd = 0;
    int accumEven = 0;
    int checkSum = 0;
    int next = 1;

    // user input
    string user_card_number = get_string("Card Number: ");
    int stringLen = strlen(user_card_number);
    char first_two_digits[3];
    strncpy(first_two_digits, user_card_number, 2);
    int first_two_numbers = atoi(first_two_digits);

    // for loop to loop through all the numbers entered by the user except the first one
    for (int i = stringLen - 1; i >= 0; i = i - 1)
    {

        // convert char to number for making operations
        int charToNumber = (user_card_number[i] - '0');

        if (next)
        {
            // summ to acumulate the even positions during the loop
            accumEven = accumEven + charToNumber;
            next = 0;
        }
        else
        {

            if (charToNumber * 2 >= 10)
            {
                // summ to acumulate the digits of the product in positions during the loop
                accumOdd = accumOdd + (charToNumber * 2) / 10 + (charToNumber * 2) % 10;
            }
            else
            {
                // summ to acumulate the odd positions during the loop
                accumOdd = accumOdd + charToNumber * 2;
            }
            next = 1;
        }
    }

    int totalAccum = accumOdd + accumEven;

    // cases
    if (first_two_digits[0] == '4' && (stringLen == 13 || stringLen == 16) && totalAccum % 10 == 0)
    {
        printf("VISA\n");
    }
    else if (first_two_numbers >= 51 && first_two_numbers <= 55 && stringLen == 16 &&
             totalAccum % 10 == 0)
    {
        printf("MASTERCARD\n");
    }
    else if ((first_two_numbers == 34 || first_two_numbers == 37) && stringLen == 15 &&
             totalAccum % 10 == 0)
    {
        printf("AMEX\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
