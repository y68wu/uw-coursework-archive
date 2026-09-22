#include "f.h"
//going to make 4 random calls to print_file functions
#include "i.h"
#include "t.h"
#include "j.h"
#include "z.h"
#include <stdio.h>

void print_file_f(){
  printf("In f\n");
}
void print_f(){
  printf("In function printf within file f.c\n");
  print_file_i();
  print_file_t();
  print_file_j();
  print_file_z();
}
