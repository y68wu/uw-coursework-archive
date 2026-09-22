#include "i.h"
//going to make 4 random calls to print_file functions
#include "c.h"
#include "h.h"
#include "r.h"
#include "w.h"
#include <stdio.h>

void print_file_i(){
  printf("In i\n");
}
void print_i(){
  printf("In function printi within file i.c\n");
  print_file_c();
  print_file_h();
  print_file_r();
  print_file_w();
}
