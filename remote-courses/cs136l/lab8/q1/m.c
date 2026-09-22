#include "m.h"
//going to make 1 random calls to print_file functions
#include "v.h"
#include <stdio.h>

void print_file_m(){
  printf("In m\n");
}
void print_m(){
  printf("In function printm within file m.c\n");
  print_file_v();
}
