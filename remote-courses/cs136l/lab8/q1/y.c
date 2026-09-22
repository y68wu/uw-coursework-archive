#include "y.h"
//going to make 2 random calls to print_file functions
#include "e.h"
#include "r.h"
#include <stdio.h>

void print_file_y(){
  printf("In y\n");
}
void print_y(){
  printf("In function printy within file y.c\n");
  print_file_e();
  print_file_r();
}
