#include "s.h"
//going to make 1 random calls to print_file functions
#include "w.h"
#include <stdio.h>

void print_file_s(){
  printf("In s\n");
}
void print_s(){
  printf("In function prints within file s.c\n");
  print_file_w();
}
