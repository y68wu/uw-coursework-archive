#include "r.h"
//going to make 4 random calls to print_file functions
#include "y.h"
#include "g.h"
#include "s.h"
#include "v.h"
#include <stdio.h>

void print_file_r(){
  printf("In r\n");
}
void print_r(){
  printf("In function printr within file r.c\n");
  print_file_y();
  print_file_g();
  print_file_s();
  print_file_v();
}
