
#include "utilities.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

const char WORDFILENAME[] = "/Users/mhu/Downloads/smallwords.txt";
int playOneRound (const char words[][MAXWORDLEN+1], int nWords, int wordnum);
bool isAllLower (const char probe[]);
bool isExist (const char words[][MAXWORDLEN + 1], int nWords, const char probe[]);
void GoldSilver (const char target[MAXWORDLEN + 1],
                 const char guess[MAXWORDLEN +1],
                 int& gold, int& silver);

int main()
{
    char wordlist[9000][MAXWORDLEN + 1];
    int n = getWords(wordlist, 9000, WORDFILENAME);
    if (n < 1) // text file was not found or contained zero words
    {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 0;
    }
    
    //*****************************
    // start the game!!!
    //*****************************
    int min = 0, max = 0;
    double total = 0;
    
    cout << "How many rounds do you want to play? ";
    int Round;
    cin >> Round;
    cin.ignore(10000, '\n');
    
    if (Round <= 0)
    {
        cout << "The number of rounds must be positive." << endl;
        return 0;
    }
    
    cout << endl;
    // the loop runs iterations one time, the game plays one round
    for (int i = 0; i < Round; i++)
    {
        cout << "Round " << i + 1 << endl;
        
        // random probe word
        int wordNum = randInt(0, n - 1);
        cout << "The hidden word is " << strlen(wordlist[wordNum]) << " letters long." << endl;
        
        // call the function to get try number
        int tryNum = playOneRound(wordlist, n, wordNum);
        if (tryNum == -1)
            return -1;                  //这里需要输出什么嘛
        else if (tryNum == 1)
            cout << "You got it in 1 try." << endl;
        else
            cout << "You got it in " << tryNum << " tries." << endl;
        
        // the first round situation
        if (i == 0)
        {
            min = tryNum;
            max = tryNum;
            total = tryNum;
        }
        // situation after the first round
        else if (i != 0)
        {
            if (tryNum < min)
                min = tryNum;
            else if (tryNum > max)
                max = tryNum;
            
            total += tryNum;
        }
        
        cout.setf(ios::fixed);
        cout.precision(2);
        // output the average time, minimum time, maximum time
        cout << "Average: " << (total / (i + 1))
             << ", minimum: " << min << ", maximum: " << max << endl;
        cout << endl;
    }
    return 0;
}

//******************************
// Function playOneRound
//******************************
int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum)
{
    int Gold = 0, Silver = 0, score = 0;
    char userInput[100];
    
    char hiddenWord[MAXWORDLEN + 1];
    strcpy(hiddenWord, words[wordnum]);
    
    if (nWords < 0 || wordnum < 0 || wordnum >= nWords)
        return -1;
    
    for (;;)
    {
        cout << "Probe word: ";
        cin.getline(userInput, 100);
        
        // check lenth of char userInput
        // check wheather all characters in string are lowercase
        if (strlen(userInput) < MINWORDLEN ||
            strlen(userInput) > MAXWORDLEN ||
            isAllLower(userInput) == false   )
        {
            cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
            continue;
        }
        
        // check wheather the input string match the probe string
        if (!isExist(words, nWords, userInput))
        {
            cout << "I don't know that word." << endl;
            continue;
        }
        
        // end the loop when user gets the answer
        if (strcmp(userInput, hiddenWord) == 0)
        {
            score++;
            break;
        }
        
        GoldSilver(hiddenWord, userInput, Gold, Silver);            //output the result
        cout << "Golds: " << Gold << ", Silvers: " << Silver << endl;
        score++;
    }
    return score;
}

//******************************
// Function GoldSilver
//******************************
void GoldSilver (const char target[MAXWORDLEN + 1],
                 const char guess[MAXWORDLEN +1],
                 int& gold, int& silver)
{
    // reset values for gold and silver when calling this function
    gold = 0;
    silver = 0;
    
    // make a copy of char target & char guess
    char hidden[MAXWORDLEN + 1];
    strcpy(hidden, target);
    char probe[MAXWORDLEN + 1];
    strcpy(probe, guess);
    
    // find gold_characters first, then count gold
    // replace these gold_characters with '&'
    for (int i = 0; i < strlen(hidden); i++)
        if (hidden[i] == probe[i])
        {
            gold++;
            hidden[i] = '&';
            probe[i] = '&';
        }
    
    // find silver_characters, then count silver
    // replace founded silver_characters with '!', to make sure count only one time
    for (int k = 0; k < strlen(hidden); k++)
        for (int n = 0; n < strlen(probe); n++)
            if ((hidden[k] == probe[n]) && k != n && isalpha(hidden[k]))
            {
                silver++;
                hidden[k] = '!';
                probe[n] = '!';
            }
}

//******************************
// Function isAllLower
//******************************
bool isAllLower (const char probe[])
{
    for (int i = 0; i < strlen(probe); i++)
    {
        if (!islower(probe[i]))
            return false;
    }
    return true;
}

//******************************
// Function isExxist
//******************************
bool isExist (const char words[][MAXWORDLEN + 1], int nWords, const char probe[])
{
    for (int i = 0; i < nWords; i++)
    {
        if (strcmp(probe, words[i]) == 0)
            return true;           
    }
    return false;
}
