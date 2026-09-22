/////////////////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT (v4)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement posted on edX:
// (Course > Assignments > Assignment Information & Policies > Academic Integrity Policy)
/////////////////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with:

// None
//
// Name: Chris Wu
// login ID: [student-id-redacted]
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hangman-io.h"
#include "words.h"
#include <stdbool.h>

/*
 * is_lower(c) checks if a character is a lowercase character.
 * The function returns true if the character is a lowercase character, false otherwise.
 */

bool is_lower(char c) {
  return c >= 'a' && c <= 'z';
}

/*
 * is_upper(c) checks if a character is a uppercase letter.
 * The function returns true if the character is a uppercase character, false otherwise.
 */

bool is_upper(char c) {
  return c >= 'A' && c <= 'Z';
}

/*
 * to_upper(c) converts a character to uppercase if it is an lowercase letter.
 * The function returns the uppercase if the character is an lowercase letter, 
 * otherwise it returns the character itself.
 */

char to_upper(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 'A';
  }
  return c;
}

/*
 * is_alphabet(c) checks if a character is an alphabet letter.
 * The function returns true if the character is a letter, false otherwise.
 */
bool is_alphabet(char c) {
  return is_lower(c) || is_upper(c);
}

/*
 * is_whitespace(c) checks if a character is a whitespace character.
 * The function returns true if the character is a whitespace character, false otherwise.
 */

static bool is_whitespace(char c) {
  return c == ' ' || c == '\n';
}

int main() {

    char playAgain = '\0';
    int strikes = 0;
    int wordLength = 0;
    int gameNumber = 0;
    char secretWord[51] = {'\0'};
    char guessedWord[51] = {'\0'};
    int guessedLetters[26] = {0};
    char guess = '\0';
    int guessedLettersCount;


    do {
        // Reset game state
        strikes = 0;
        guessedLettersCount = 0;

        // Prompt user to enter a game number
        do {
            printf(enter_game_number, max_words);
            assert(scanf("%d", &gameNumber) > 0);
            
            if (gameNumber < 1 || gameNumber > max_words) {
                printf("%s", invalid_game_number);  
            }

        } while (gameNumber < 1 || gameNumber > max_words);

        // Get the secret word

        wordLength = strlen(get_word(gameNumber));
            
        for (int i = 0; i < wordLength; i++) {
            secretWord[i] = *(get_word(gameNumber) + i);
            if (is_whitespace(secretWord[i])) {
                guessedWord[i] = secretWord[i];
            } else {
                guessedWord[i] = '*';
            }
        }
            
        secretWord[wordLength] = '\0';
        guessedWord[wordLength] = '\0';

        // Game loop
        while (strikes < max_strikes && strcmp(secretWord, guessedWord) != 0) {
            print_hangman(strikes);
            printf("%s", letter_prompt);
            printf("%s\n", guessedWord);
            scanf(" %c", &guess);

            if (!is_alphabet(guess)) {
                printf(not_a_letter, guess);
                continue;
            }

            // check if the letter is already guessed
            guess = to_upper(guess);
            bool alreadyGuessed = false;
            for (int i = 0; i < guessedLettersCount; i++) {
                if (guessedLetters[i] == guess) {
                    alreadyGuessed = true;
                    break;
                }
            }

            if (alreadyGuessed) {
                printf(already_guessed_letter, guess);
                continue;
            } else {
                guessedLetters[guessedLettersCount] = guess;
                guessedLettersCount += 1;
            }

            // check if the word is a part of the letter
            bool found = false;
            for (int i = 0; i < wordLength; i++) {
                if (secretWord[i] == guess) {
                    guessedWord[i] = guess;
                    found = true;
                }
            }

            if (!found) {
                strikes += 1;
                printf(not_in_word, guess);
                if (strikes < max_strikes) {
                    printf(not_last_guess, max_strikes - strikes);
                }
            }
        }

        // End of game
        if (strcmp(secretWord, guessedWord) == 0) {
            printf(congratulations, secretWord);
        } else {
            print_hangman(strikes);
            printf(game_over, secretWord);
        }

        // Prompt to play again
        do {
            printf("%s", play_again);
            scanf(" %c", &playAgain);
            playAgain = to_upper(playAgain);
            if (playAgain != 'Y' && playAgain != 'N') {
                printf("%s", invalid_play_again_response);
            }
        } while (playAgain != 'Y' && playAgain != 'N');

    } while (playAgain == 'Y');
}
