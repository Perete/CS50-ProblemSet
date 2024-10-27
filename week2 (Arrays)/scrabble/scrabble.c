#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int letter_score[26] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                        1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int player_score[2];

string players_words[2];

int wordScore(string word);

int main(void)
{
    for (int i = 0; i < 2; i++)
    {
        players_words[i] = get_string("Player%d: ", i + 1);
    }

    player_score[0] = wordScore(players_words[0]);
    player_score[1] = wordScore(players_words[1]);
    // printf("Score1: %d\n",player_score[0]);
    // printf("Score2: %d\n",player_score[1]);

    if (player_score[0] > player_score[1])
    {
        printf("Player 1 wins!\n");
    }
    else if (player_score[0] < player_score[1])
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int wordScore(string word)
{
    int score = 0;
    int score_index = 0;
    int word_length = strlen(word);

    for (int i = 0; i < word_length; i++)
    {
        if (isupper(word[i]))
        {
            score_index = word[i] - 'A';
            score = score + letter_score[score_index];
        }
        else if (islower(word[i]))
        {
            score_index = toupper(word[i]) - 'A';
            score = score + letter_score[score_index];
        }
    }

    return score;
}
