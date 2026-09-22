/**
 File: test-string-fun.c
 A basic test client for the string-fun module
*/

#include <assert.h>
#include <string.h>
#include "string-fun.h"
#include "cs136-trace.h"

int main(void) {
  char string1[] = "Carrot13!";

  assert(valid_password(string1));

  char string3[] = "Scissors sizzle, thistles sizzle...";


  assert(count_words(string3) == 4);


  char string4[] = "+/+";


  assert(is_palindrome_black(string4));
  reverse(string4);
  assert(0 == strcmp(string4, "+/+"));

  char string5[] = "Abba";
  char string6[] = "taco cat";

  assert(is_palindrome_gold(string5));
  assert(is_palindrome_gold(string6));
  
  char str1[] = " abc abc &*&*& ";
  reverse_words(string3);
  reverse_words(str1);
  assert(strcmp(str1, " &*&*& cba cba "));
  assert(0 == strcmp(string3, "srossicS ,elzzis seltsiht ...elzzis"));
  
  
  char *strings[4] = {"hi\n", "h i !", "love", "hello!"};

  sort_strings(strings, 4);
  assert(!strcmp(strings[0], "h i !"));
  assert(!strcmp(strings[1], "hello!"));
  assert(!strcmp(strings[2], "hi\n"));
  assert(!strcmp(strings[3], "love"));

  char string7[] = "Alphabetic be my words";
  char string8[] = "My words are alphabetic";
  assert(is_alphabetic(string7));
  assert(!is_alphabetic(string8));

  char string10[] = "..a,,,B,,,C,,,d..";
  char string11[] = "....";
  char string12[] = " abc ";
  char string13[] = "....Defff,.. gEC,.,.,L.,.,.r";
  char string14[] = "QBCE";
  char string15[] = "&&&Deff&&&gEC&&&L&&r";
  char string16[] = "....Defff,.. gEC,.,.,L.,.,.a";
  

  assert(is_alphabetic(string10));
  assert(is_alphabetic(string11));
  assert(is_alphabetic(string12));
  assert(is_alphabetic(string13));
  assert(is_alphabetic(string14));
  assert(is_alphabetic(string15));
  assert(!is_alphabetic(string16));

  char string17[] = "aaa  AAAA  BBBB";
  assert(!is_alphabetic(string17));

  char string9[] = "A random assortment of text";

  assert(substring(string9, "sort") == 11);
  assert(substring(string9, "cat") == -1);


}
