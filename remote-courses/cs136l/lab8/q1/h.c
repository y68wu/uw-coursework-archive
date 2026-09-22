#include "h.h"
//going to make 1 random calls to print_file functions
#include "n.h"
#include <stdio.h>

void print_file_h(){
  printf("In h\n");
}
void print_h(){
  printf("In function printh within file h.c\n");
  print_file_n();
}
