#include <stdbool.h>
//  A module for some fun string activities

// all of the following functions require valid string parameters
// all strings consist of printable characters [not asserted]

// Printable characters are characters with ASCII values 32-126 inclusive,
// plus the newline character (ASCII value 10)


///////////////////////////////////////////////////////////////////////////////
// BLACK QUESTIONS
///////////////////////////////////////////////////////////////////////////////


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
bool valid_password(const char *s);


// count_words(s) returns the number of words in string s.  Reminder:
//   a word is a maximal sequence of non-whitespace characters, i.e.
//   what scanf("%s", ...) reads
// time: O(n)
int count_words(const char *s);


// reverse(s) reverses the string s
// time: O(n)
// effects: s is modified
void reverse(char *s);


// is_palindrome_black(s) returns true if s is a palindrome, meaning
//  it is equal to its reverse.  e.g. "abba" is a palindrome
//  as is "+/+".  "Abba" is not because its reverse is "abbA"
// time: O(n)
bool is_palindrome_black(const char *s);


///////////////////////////////////////////////////////////////////////////////
// GOLD QUESTIONS
///////////////////////////////////////////////////////////////////////////////

// is_palindrome_gold(s) returns true if s is a palindrome.
//   This version only considers letters
//   (i.e it ignores all non-letter characters), and is case insensitive
// e.g. the following are palindromes for this question, but would not be
//      for the previous version
//      "Abba", "taco cat"
// time: O(n)

bool is_palindrome_gold(const char *s);

// reverse_words(s) reverses each word in s, but it does NOT reverse the order
//   of the words themselves, e.g. "taco cat" becomes "ocat tac"
// time: O(n)
// effects: s is modified

void reverse_words(char *s);


// sort_strings(aos,len) sorts aos into ascending lexicographic order
// note: aos is an array of strings
//       len is the length of aos
// effects: aos might be reordered
// time: O(m * n^2) where m is the length of the longest string in aos
//                  and n is the length of aos

void sort_strings(char **aos, int len);


// is_alphabetic(s) returns true if and only if the individual words of s
//  are "alphabetic", meaning the first letter of each word in s
//  is strictly increasing.  This is case-insensitive
// example: "Alphabetic be my words" => true (abmw is in alphabetic order)
// requires: each word starts with a letter [not asserted]
// time: O(n)

bool is_alphabetic(const char *s);


// substring(haystack, needle) returns the index of the first occurance of
//  [needle] in [haystack], or -1 if [needle] does not occur
// time: O(nm) where n is strlen(haystack) and m is strlen(needle)

int substring(const char *haystack, const char *needle);
