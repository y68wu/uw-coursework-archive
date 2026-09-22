// student.h [INTERFACE]

#include <stdbool.h>

struct student {
  int id;
  int cs_courses;
  int total_courses;
  int cs_grades;
  int total_grades;
  int failure;
  int cs_failure;
};

// NOTE: All of the following functions REQUIRE:
//       pointers to a student (e.g., s) are not NULL
//       grades are between 0 and 100 (inclusive)

// All calculated averages are rounded down
// For example, the average of 70 and 73 is 71

// A failure is a grade below 50


// initialize_new_student(s) assigns initial values to the student s
//   but leaves the s->id field unchanged
// effects: modifies s
void initialize_new_student(struct student *s);


// take_course(&s, grade, cs) updates the student s to reflect that they took
//   a course with the specified grade.  cs determines if it was a cs course.
// effects: modifies s
void take_course(struct student *s, int grade, bool cs);


// adjust_grade(s, original_grade, new_grade, cs) updates the student s 
//   to reflect that a previous original_grade has been adjusted (changed)
//   to now be new_grade.  cs determines if it was a cs course.
// requires: a course was previously taken with original_grade
//           note: you should not (and cannot) enforce this requirement
// effects: modifies s
void adjust_grade(struct student *s, int original_grade, 
                  int new_grade, bool cs);

                  
// cs_average(s) determines the average of all CS courses taken by s
//   (or 0 if none taken)
int cs_average(const struct student *s);


// cav(s) determines the Cumulative AVerage (CAV) of all courses taken by s
//  (or 0 if none taken)
int cav(const struct student *s);


// print_status(s) prints out the status code of student s, followed by \n
//   The status code is a 3 or 4 letter code in capital letters.
//   Note: this is an oversimplified view of the real UW MATH codes
//   If multiple codes apply, the one highest in the following list is used:
//     NONE: number of courses = 0          [no status]
//     FRW:  number of failed courses > 4   [required to withdraw]
//     FRW:  # of fails > # of passes       [required to withdraw]
//     NOCS: number of cs courses = 0       [no cs status]
//     FLPP: CS_AVG < 60 and CAV < 60       [must leave plan]
//     PRBO: CAV < 60                       [probation]
//     CNDP: CS_AVG < 60                    [conditional standing]
//     GOOD: 60 <= CAV < 80                 [good]
//     EXCL: CAV >= 80                      [excellent]
// effects: displays a message
void print_status (const struct student *s);


// print_student(&s) prints the details of student s formatted exactly as 
//   illustrated by the following example:
//   "Student: 12345, CAV: 66, CS_AVG: 55, Failures: 1, Status: CNDP\n"
// effects: displays a message
void print_student(const struct student *s);


// student_equals(&s1, &s2) determines if the two student records are
//   identical 
bool student_equals(const struct student *s1, const struct student *s2);
// Motivation: C does not provide a function to check whether two instances of
//   of a user-defined type are equal. Therefore, we must implement our own. 
//   Checking for equality does not mean checking whether the pointers
//   point to the same instance, but rather checking the contents of each
//   field for equality.  (checking just the id is also not sufficient)


// can_graduate(&s) determines if student s is eligible for graduation.
//   To graduate, a student must:
//     * have passed at least 40 courses, at least 15 of which must be CS
//     * have a status of GOOD or EXCL (see above)
bool can_graduate(const struct student *s);