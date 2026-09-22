#include "b.h"
//going to make 3 random calls to print_file functions
#include "z.h"
#include "q.h"
#include "k.h"
#include <stdio.h>

void print_file_b(){
  printf("In b\n");
}
void print_b(){
  printf("In function printb within file b.c\n");
  print_file_z();
  print_file_q();
  print_file_k();
}
