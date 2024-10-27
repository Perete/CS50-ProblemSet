#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool keyCheck(string key);
char* cypherText(string plainText, string key);

int main(int argc, string argv[])
{
    if (argc == 2 && strlen(argv[1]) == 26){

        // CHECK KEY IS CORRECT
        bool keyPass = keyCheck(argv[1]);

        if (!keyPass){
            return 1;
        }

        // ASK USER FOR TEXT TO CYPHER
        string plainText = get_string("Plaintext: ");

        // CYPHER TEXT
        char* secretText = cypherText(plainText, argv[1]);

        // PRINT CYPHER TEXT
        printf("ciphertext: %s\n", secretText);
        return 0;

    }else if (argc == 2 && strlen(argv[1]) != 26){

        printf("Key must be 26 character length\n");
        return 1;

    }else{

        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
}

bool keyCheck(string key){
    int key_index_sum = 0;
    int character_counter[26];

    for (int i = 0, n=strlen(key); i < n; i++){

        if (isalpha(key[i])){

            if (islower(key[i])){
                key_index_sum = key[i] - 'a';

            }else{
                key_index_sum = key[i] - 'A';
            }

            character_counter[key_index_sum]++;

            if (character_counter[key_index_sum] > 1){
                printf("Caracter repetido varias veces\n");
                return false;
            }
        }else{
            printf("Caracter no permitido");
            return false;
        }
    }

    return true
}

char* cypherText(string plainText, string key){
    int key_index = 0;
    // char cypherText[strlen(plainText) + 1];
    char* cypherText = malloc(strlen(plainText) + 1);

    for (int i = 0, n=strlen(plainText); i < n; i++){

        if (isalpha(plainText[i])){

            if (isupper(plainText[i])){

                key_index = plainText[i] - 'A';

                if (isupper(key[key_index])){
                    cypherText[i] = key[key_index];

                }else{
                    cypherText[i] = toupper(key[key_index]);
                }

            }else if (!isupper(plainText[i])){

                key_index = plainText[i] - 'a';

                if (isupper(key[key_index])){
                    cypherText[i] = tolower(key[key_index]);

                }else{
                    cypherText[i] = key[key_index];
                }
            }
        }else{
            cypherText[i] = plainText[i];
        }
    }

    cypherText[(strlen(plainText))] = '\0';

    return cypherText;
}
