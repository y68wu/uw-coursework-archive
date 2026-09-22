#include "l.h"
//going to make 1 random calls to print_file functions
#include "d.h"
#include <stdio.h>

void print_file_l(){
  printf("In l\n");
}
void print_l(){
  printf("In function printl within file l.c\n");
  print_file_d();
}
