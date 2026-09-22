#include "p.h"
//going to make 1 random calls to print_file functions
#include "p.h"
#include <stdio.h>

void print_file_p(){
  printf("In p\n");
}
void print_p(){
  printf("In function printp within file p.c\n");
  print_file_p();
}
