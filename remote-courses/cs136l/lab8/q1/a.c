#include "a.h"
//going to make 3 random calls to print_file functions
#include "s.h"
#include "g.h"
#include "d.h"
#include <stdio.h>

void print_file_a(){
  printf("In a\n");
}
void print_a(){
  printf("In function printa within file a.c\n");
  print_file_s();
  print_file_g();
  print_file_d();
}
