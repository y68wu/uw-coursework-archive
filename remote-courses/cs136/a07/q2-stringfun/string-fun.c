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

#include "string-fun.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////
// BLACK QUESTIONS
///////////////////////////////////////////////////////////////////////////////

/*
 * is_lower(c) checks if a character is a lowercase character.
 * The function returns true if the character is a lowercase character, false otherwise.
 */

static bool is_lower(char c) {
  return c >= 'a' && c <= 'z';
}

/*
 * is_upper(c) checks if a character is a uppercase letter.
 * The function returns true if the character is a uppercase character, false otherwise.
 */

static bool is_upper(char c) {
  return c >= 'A' && c <= 'Z';
}

/*
 * is_digit(c) checks if a character is a digit.
 * The function returns true if the character is a valid digit, false otherwise.
 */

static bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

/*
 * is_whitespace(c) checks if a character is a whitespace character.
 * The function returns true if the character is a whitespace character, false otherwise.
 */

static bool is_whitespace(char c) {
  return c == ' ' || c == '\n';
}

/*
 * is_special_character(c) checks if a character is a special character.
 * A special character is defined as a printable character that is not a
 * lowercase letter, uppercase letter, digit, or whitespace.
 * The function returns true if the character is a special character, false otherwise.
 */

bool is_special_character(char c) {
    return !is_lower(c) && !is_upper(c) && !is_digit(c) && !is_whitespace(c);
}

// valid_password(s) returns true if s follows the arbitrary and capricious
//   rules for a password.  The rules are as follows:
//   * the length is at least 8
//   * it contains at least one lower case letter 
//   * it contains at least one capital letter 
//   * it contains at least one digit
//   * it does NOT contain whitespace (i.e. ' ' or '\n')
//   * it contains at least one "special" character, where a special character
//     is a printable character other than those mentioned above (e.g. '$')
//     (see above for what printable characters are)
// time: O(n)

bool valid_password(const char *s) {
  assert(s);

  if (strlen(s) < 8) {
    return false;
  }

  bool has_lower = false;
  bool has_upper = false;
  bool has_digit = false;
  bool has_special = false;

  for (int i = 0; *(s + i) != '\0'; i++) {
    if (is_lower(*(s + i))) {
      has_lower = true;
      } else if (is_upper(*(s + i))) {
        has_upper = true;
      } else if (is_digit(*(s + i))) {
        has_digit = true;
      } else if (is_whitespace(*(s + i))) {
        return false;  
        break;
      } else if (is_special_character(*(s + i))) {
        has_special = true;
      }
  }

  return has_lower && has_upper && has_digit && has_special;
}


// count_words(s) returns the number of words in string s.  Reminder:
//   a word is a maximal sequence of non-whitespace characters, i.e.
//   what scanf("%s", ...) reads
// time: O(n)

int count_words(const char *s) {
  assert(s);

  int count = 0; // count the number of words

  while (*s) {

    while (*s && is_whitespace(*s)) {
      s += 1;
    }

    if (*s == '\0') {
      break;
    } else {
      count += 1;
    }
    
    while (*s && !is_whitespace(*s)) {
      s += 1;
    }
  }
  
  return count;
}


// reverse(s) reverses the string s
// time: O(n)
// effects: s is modified

void reverse(char *s) {
  assert(s);

  int left = 0;
  int right = strlen(s) - 1;

  while (left < right) {
    char temp = s[left];
    *(s + left) = *(s + right);
    *(s + right) = temp;
    left += 1;
    right -= 1;
  }
}

// is_palindrome_black(s) returns true if s is a palindrome, meaning
//  it is equal to its reverse.  e.g. "abba" is a palindrome
//  as is "+/+".  "Abba" is not because its reverse is "abbA"
// time: O(n)

bool is_palindrome_black(const char *s) {
  assert(s);

  int left = 0;
  int right = strlen(s) - 1;
  
  while (left < right) {
    if (* (s + left) != *(s + right)) {
      return false;
      break;
    }
    left++;
    right--;
  }
  
  return true;
}

/*
































THIS IS INTEGRITY GOOSE. 
INTEGRITY GOOSE IS HERE TO MAKE SURE YOU DO NOT ACCIDENTIALLY SHOW SOMEONE 
YOUR GOLD CODE WHILE TRYING TO ONLY SHOW THEM YOUR BLACK CODE!



                                                        _...--.
                                        _____......----'     .'
                                  _..-''                   .'
                                .'                       ./
                        _.--._.'                       .' |
                     .-'                           .-.'  /
                   .'   _.-.                     .  \   '
                 .'  .'   .'    _    .-.        / `./  :
               .'  .'   .'  .--' `.  |  \  |`. |     .'
            _.'  .'   .' `.'       `-'   \ / |.'   .'
         _.'  .-'   .'     `-.            `      .'
       .'   .'    .'          `-.._ _ _ _ .-.    :
      /    /o _.-'     LGB       .--'   .'   \   |
    .'-.__..-'                  /..    .`    / .'
  .'   . '                       /.'/.'     /  |
 `---'                                   _.'   '
                                       /.'    .'
                                        /.'/.'




DO NOT BE ALARMED!
IF YOU MAKE NO SUDDEN MOVES, INTEGRITY GOOSE WILL NOT BITE





































*/

///////////////////////////////////////////////////////////////////////////////
// GOLD QUESTIONS
///////////////////////////////////////////////////////////////////////////////

/*
 * to_lower(c) converts a character to lowercase if it is an uppercase letter.
 * The function returns the lowercase if the character is an uppercase letter, 
 * otherwise it returns the character itself.
 */

static char to_lower(char c) {
  if (c >= 'A' && c <= 'Z') {
    return c - 'A' + 'a';
  }
  return c;
}

/*
 * is_alphabet(c) checks if a character is an alphabet letter.
 * The function returns true if the character is a letter, false otherwise.
 */
static bool is_alphabet(char c) {
  return is_lower(c) || is_upper(c);
}

// is_palindrome_gold(s) returns true if s is a palindrome.
//   This version only considers letters
//   (i.e it ignores all non-letter characters), and is case insensitive
// e.g. the following are palindromes for this question, but would not be
//      for the previous version
//      "Abba", "taco cat"
// time: O(n)

bool is_palindrome_gold(const char *s) {
  assert(s);

  int left = 0;
  int right = strlen(s) - 1;

  while (left < right) {

    while (left < right && !is_alphabet(*(s + left))) {
      left += 1;
    }
    
    while (left < right && !is_alphabet(* (s + right))) {
      right -= 1;
    }

    if (to_lower(*(s + left)) != to_lower(*(s + right))) {
      return false;
      break;
    }

    left += 1;
    right -= 1;
  }

  return true;
}

/*
 * part_reverse(start, end) reverses the characters in the string from start to end.
 */

static void part_reverse(char *start, char *end) {
  assert(start);
  assert(end);

  while (start < end) {
    char temp = *start;
    *start = *end;
    *end = temp;
    start += 1;
    end -= 1;
  }
}

// reverse_words(s) reverses each word in s, but it does NOT reverse the order
//   of the words themselves, e.g. "taco cat" becomes "ocat tac"
// time: O(n)
// effects: s is modified

void reverse_words(char *s) {
  assert(s);

  char *start = s;

  while (*start) {

    while (is_whitespace(*start) && *start) {
      start += 1;
    }

    if (*start == '\0') {
      break;
    }
    
    char *end = start;

    while (!is_whitespace(*end) && *end) {
      end += 1;
    }
    
    part_reverse(start, end - 1);
    
    start = end;
  }
} 


// sort_strings(aos,len) sorts aos into ascending lexicographic order
// note: aos is an array of strings
//       len is the length of aos
// effects: aos might be reordered
// time: O(m * n^2) where m is the length of the longest string in aos
//                  and n is the length of aos

void sort_strings(char **aos, int len) {
  assert(aos);
  assert(len > 0);

  for (int i = 0; i < len - 1; i++) {
    for (int j = 0; j < len - i - 1; j++) {
      if (strcmp(aos[j], aos[j + 1]) > 0) {
        char *temp = aos[j];
        aos[j] = aos[j + 1];
        aos[j + 1] = temp;
      }
    }
  }

}


// is_alphabetic(s) returns true if and only if the individual words of s
//  are "alphabetic", meaning the first letter of each word in s
//  is strictly increasing.  This is case-insensitive
// example: "Alphabetic be my words" => true (abmw is in alphabetic order)
// requires: each word starts with a letter [not asserted]
// time: O(n)

bool is_alphabetic(const char *s) {
    assert(s);

    char prev_letter = '\0';
    bool first_word = true;

    while (*s) {

        while (*s && !is_alphabet(*s)) {
          s += 1;
        }

        if (*s == '\0') {
          break;
        }

        char current_letter = to_lower(*s);

        if (!first_word && current_letter <= prev_letter) {
          return false;
          break;
        }

        prev_letter = current_letter;
        first_word = false;

        while (*s && is_alphabet(*s)) {
          s += 1;
        }
    }

    return true;
}

// substring(haystack, needle) returns the index of the first occurance of
//  [needle] in [haystack], or -1 if [needle] does not occur
// time: O(nm) where n is strlen(haystack) and m is strlen(needle)

int substring(const char *haystack, const char *needle) {
  assert(haystack);
  assert(needle);
  
  int haystack_len = strlen(haystack);
  int needle_len = strlen(needle);

  if (needle_len == 0) {
    return 0;
  }

  if (needle_len > haystack_len) {
    return -1; 
  }

  for (int i = 0; i < haystack_len; i++) {
    bool found = true;
    for (int j = 0; j < needle_len; j++) {
      if (haystack[i + j] != needle[j]) {
        found = false;
        break;
      }
    }
    if (found) {
      return i;
    }
  }

  return -1; 
}
