#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int letter_counter = 0;
int word_counter = 0;
int sentence_counter = 0;
int comma_counter = 0;

int main(void){

    string userInput = get_string("Text: ");

    for(int i=0, n=strlen(userInput); i<=n; i++)
    {
        if(isalpha(userInput[i])){
            letter_counter++;

        }else if (userInput[i] == ' ' || userInput[i] == '\0'){
            word_counter++;

        }else if(userInput[i] == '.' || userInput[i] == '?' || userInput[i] == '!'){
            sentence_counter++;
        }
    }

    // letter_counter = strlen(userInput) - word_counter + 1 - sentence_counter - comma_counter;
    printf("Length: %lu\n", strlen(userInput));
    printf("Letters: %d\n", letter_counter);
    printf("Words: %d\n",word_counter);
    printf("Sentences: %d\n", sentence_counter);

    float avg_letters = letter_counter/(float) word_counter;
    float L = avg_letters*100;

    float avg_sentence = sentence_counter/(float) word_counter;
    float S = avg_sentence*100;

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if(index < 1){
        printf("Before Grade 1\n");
    }else if (index > 16){
        printf("Grade 16+\n");
    }else{
        printf("Grade %d\n", index);
    }
}
