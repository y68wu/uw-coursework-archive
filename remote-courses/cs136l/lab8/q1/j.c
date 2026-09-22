#include "j.h"
//going to make 5 random calls to print_file functions
#include "j.h"
#include "y.h"
#include "v.h"
#include "f.h"
#include "m.h"
#include <stdio.h>

void print_file_j(){
  printf("In j\n");
}
void print_j(){
  printf("In function printj within file j.c\n");
  print_file_j();
  print_file_y();
  print_file_v();
  print_file_f();
  print_file_m();
}
