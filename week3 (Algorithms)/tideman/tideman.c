#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void print_array(int array[]);

bool returnPath(int nodeStart, int nodeEnd);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        // printf("Ranks: ");
        // print_array(ranks);
        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}


/*  Given a name by the user, the function vote() update the array ranks checking that the name introduced by the user
    Match with the available candidates, if match then the preference order is save.
    When user end ranking candidates you could see the array rank like this:

    ranks[] = [1, 0, 2]
    Where candidate 1 is in array position 0 and this position is the rank 1. Candidate 1 in position 0 in the array
    is prefered over candidate 0 that it is in array position 1 and therefore rank2...

    To sum up:
    ranks[0] = Candidate 1 is prefered over Candidate 0, and candidate 2
    ranks[1] = Candidate 0 is prefered over Candidate 2
    ranks[2] = Candidate 2 is not prefered over any candidate
*/
// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i=0; i<candidate_count; i++){
        if(strcmp(name, candidates[i]) == 0){
            // printf("El nombre %s coincide con %s\n", name, candidates[i]);
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

/*  Update preferences based in the rank introduced by the user.
               [0] [1] [2]
    ranks[] = [ 1,  0,  2 ]
    ranks[0] = Candidate 1 is prefered over Candidate 0, and candidate 2
    ranks[1] = Candidate 0 is prefered over Candidate 2
    ranks[2] = Candidate 2 is not prefered over any candidate

    *Preferences is a bidimensional array where the candidate i is prefered n times over candidate j.

    preferences[i][j]=      [0] [1] [2]
                     [0]   [ 0,  0,  1 ]
                     [1]   [ 1,  0,  1 ]
                     [2]   [ 0,  0,  0 ]

    Using the ranks as index in preferences --> preferences[ranks[i]] [ranks[j]]
    The second loop depend of the first one.
*/

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for(int i=0; i<candidate_count; i++){
        int n = ranks[i];
        for(int j=i; j<candidate_count; j++){
            int m = ranks[j];
            if(m == n){
                preferences[n][m] = 0;
            }else{
                preferences[n][m]++;
            }
        }
    }

    // printf("Preferences: \n");
    // for(int i=0; i<candidate_count; i++){
    //     print_array(preferences[i]);
    // }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for(int i=0; i<candidate_count; i++){
         for(int j=i; j<candidate_count; j++){
            if(i!=j){
                if( preferences[i][j] > preferences[j][i]){
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }else if(preferences[i][j] < preferences[j][i]){
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                    pair_count++;
                }
            }
         }
    }
    // for(int i=0; i<pair_count; i++){
    //     printf("Winner: %d\n", pairs[i].winner);
    //     printf("Loser: %d\n", pairs[i].loser);
    // }
    // printf("pair_count: %d\n", pair_count);
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for(int i=0; i<pair_count;i++){
        int minIndex = i;
        int minDiff = preferences[pairs[minIndex].winner][pairs[minIndex].loser] - preferences[pairs[minIndex].loser][pairs[minIndex].winner];
        for(int j=i+1; j<pair_count; j++){
            int diff = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];

            if(diff > minDiff){
                minIndex = j;
                minDiff = diff;
            }
        }

        if(minIndex != i){
            pair temp = pairs[minIndex];
            pairs[minIndex] = pairs[i];
            pairs[i] = temp;
        }
    }

    // for(int i=0; i<pair_count; i++){
    //     printf("Winner: %d\n", pairs[i].winner);
    //     printf("Loser: %d\n", pairs[i].loser);
    // }
    // printf("pair_count: %d\n", pair_count);
    // printf("\n");

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for(int i=0; i<pair_count; i++){
        if(!returnPath(pairs[i].winner, pairs[i].loser)){
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    // printf("Locked: \n");
    // for(int i=0; i<pair_count; i++){
    //     printf("[ ");
    //     for(int j=0; j<pair_count; j++){
    //         printf("%d ",locked[i][j]);
    //     }
    //     printf("]\n");
    // }

    return;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    for(int i=0; i<pair_count; i++){
        int acc = 0;
        for(int j=0; j<pair_count; j++){
            if(locked[j][i]){
                break;
            }else{
                acc++;
            }
        }
        if(acc == pair_count){
            // printf("\nEl ganador es el candidato %d, %s\n", i, candidates[i]);
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}

void print_array(int array[]){

    // printf("Size: %d\n", candidate_count);
    printf("[ ");
    for(int i=0; i<candidate_count; i++){
        printf("%d ",array[i]);
    }
    printf("]\n");
    // for(int i=0; )
}

bool returnPath(int nodeStart, int nodeEnd){

    if(nodeStart == nodeEnd){
        return true;
    }

    for(int i=0; i<pair_count; i++){
        if(locked[i][nodeStart]){
            if(returnPath(i, nodeEnd)){
                return true;
            };
        }
    }

    return false;
}
