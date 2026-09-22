#include "q.h"
//going to make 2 random calls to print_file functions
#include "o.h"
#include "b.h"
#include <stdio.h>

void print_file_q(){
  printf("In q\n");
}
void print_q(){
  printf("In function printq within file q.c\n");
  print_file_o();
  print_file_b();
}
