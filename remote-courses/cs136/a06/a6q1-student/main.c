#include <assert.h>
#include "student.h"
#include <stdio.h>

// this is a very basic test client
// you should test your code more thoroughly

int main(void) {
  struct student s = {.id = 12345};
  initialize_new_student(&s);
  print_status(&s);
  print_student(&s);
  assert(student_equals(&s, &s));
  take_course(&s, 70, true);
  take_course(&s, 72, false);
  adjust_grade(&s, 72, 73, false);
  assert(cs_average(&s) == 70);
  assert(cav(&s) == 71);
  assert(!can_graduate(&s));
  struct student s1 = {.id = 1};
  struct student s2 = {.id = 1};
  initialize_new_student(&s1);
  initialize_new_student(&s2);
  assert(student_equals(&s1, &s2));
  take_course(&s1, 81, true);
  take_course(&s2, 81, true);
  print_student(&s1);
  print_student(&s2);
  assert(student_equals(&s1, &s2));
  adjust_grade(&s1, 81, 30, true);
  print_student(&s1);
  take_course(&s1, 90, false);
  print_student(&s1);
  assert(!can_graduate(&s1));
  take_course(&s1, 40, false);
  print_student(&s1);
  adjust_grade(&s1, 40, 60, false);
  print_student(&s1);
}