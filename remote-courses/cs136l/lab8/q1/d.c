#include "d.h"
//going to make 4 random calls to print_file functions
#include "r.h"
#include "t.h"
#include "v.h"
#include "r.h"
#include <stdio.h>

void print_file_d(){
  printf("In d\n");
}
void print_d(){
  printf("In function printd within file d.c\n");
  print_file_r();
  print_file_t();
  print_file_v();
  print_file_r();
}
