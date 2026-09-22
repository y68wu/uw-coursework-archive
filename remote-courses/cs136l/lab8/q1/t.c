#include "t.h"
//going to make 4 random calls to print_file functions
#include "j.h"
#include "e.h"
#include "g.h"
#include "p.h"
#include <stdio.h>

void print_file_t(){
  printf("In t\n");
}
void print_t(){
  printf("In function printt within file t.c\n");
  print_file_j();
  print_file_e();
  print_file_g();
  print_file_p();
}
