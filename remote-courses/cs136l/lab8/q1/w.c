#include "w.h"
//going to make 2 random calls to print_file functions
#include "p.h"
#include "a.h"
#include <stdio.h>

void print_file_w(){
  printf("In w\n");
}
void print_w(){
  printf("In function printw within file w.c\n");
  print_file_p();
  print_file_a();
}
