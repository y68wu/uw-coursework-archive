#include "u.h"
//going to make 3 random calls to print_file functions
#include "s.h"
#include "l.h"
#include "u.h"
#include <stdio.h>

void print_file_u(){
  printf("In u\n");
}
void print_u(){
  printf("In function printu within file u.c\n");
  print_file_s();
  print_file_l();
  print_file_u();
}
