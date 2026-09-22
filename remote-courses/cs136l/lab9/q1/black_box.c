#include "black_box.h"


void rab(int *a){
  for (int i=0; i < *a; ++i){
    *a = *a - i;
  }
}

void bar(int **p){
  int *q = *p;
  if (*q > 5){
    *q = *q - 3;
  }
  rab(q);
}

void oof(int original, int updated, int *ptr){
  if (original > *ptr){
    *ptr = original;
  } 
  *ptr -= *ptr + updated - 1; 
  
}

void foo(int *z){
  int temp1 = *z;
  bar(&z);
  int temp2 = *z;
  --(*z);
  oof(temp1,temp2,z);
}

