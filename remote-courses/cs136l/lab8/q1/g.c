#include "g.h"
//going to make 1 random calls to print_file functions
#include "a.h"
#include <stdio.h>

void print_file_g(){
  printf("In g\n");
}
void print_g(){
  printf("In function printg within file g.c\n");
  print_file_a();
}
