#include "z.h"
//going to make 1 random calls to print_file functions
#include "a.h"
#include <stdio.h>

void print_file_z(){
  printf("In z\n");
}
void print_z(){
  printf("In function printz within file z.c\n");
  print_file_a();
}
