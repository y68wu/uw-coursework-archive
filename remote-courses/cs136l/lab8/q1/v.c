#include "v.h"
//going to make 2 random calls to print_file functions
#include "m.h"
#include "p.h"
#include <stdio.h>

void print_file_v(){
  printf("In v\n");
}
void print_v(){
  printf("In function printv within file v.c\n");
  print_file_m();
  print_file_p();
}
