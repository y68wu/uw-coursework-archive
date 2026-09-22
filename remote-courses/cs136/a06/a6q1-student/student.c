/////////////////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT (v4)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement posted on edX:
// (Course > Assignments > Assignment Information & Policies > Academic Integrity Policy)
/////////////////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with:

// None
//
// Name: Chris Wu
// login ID: [student-id-redacted]
//////////////////////////////////////////////////////////////////////////////////////////

// student.c [IMPLEMENTATION]

#include <stdio.h>
#include "student.h"
#include "assert.h"

// initialize_new_student(s) assigns initial values to the student s
// but leaves the s->id field unchanged
// effects: modifies s
void initialize_new_student(struct student *s) {
    assert(s);
    s->cs_courses = 0;
    s->cs_grades = 0;
    s->failure = 0;
    s->cs_failure =0;
    s->total_courses = 0;
    s->total_grades = 0;
}

// take_course(&s, grade, cs) updates the student s to reflect that they took
// a course with the specified grade.  cs determines if it was a cs course.
// effects: modifies s
void take_course(struct student *s, int grade, bool cs) {
    assert(s);
    assert(grade >= 0);
    assert(grade <= 100);

    if (cs) {
        if (grade < 50) {
            s->failure += 1;
            s->cs_failure +=1;
            }
        s->total_courses += 1;
        s->cs_courses += 1;
        s->cs_grades += grade;
        s->total_grades += grade;
    } else {
        if (grade < 50) {
            s->failure += 1;
            }
        s->total_courses += 1;
        s->total_grades += grade;
    }
}

// adjust_grade(s, original_grade, new_grade, cs) updates the student s 
//   to reflect that a previous original_grade has been adjusted (changed)
//   to now be new_grade.  cs determines if it was a cs course.
// requires: a course was previously taken with original_grade
//           note: you should not (and cannot) enforce this requirement
// effects: modifies s
void adjust_grade(struct student *s, int original_grade, 
                  int new_grade, bool cs) {
                    assert(s);
                    assert(original_grade >= 0);
                    assert(original_grade <= 100);
                    assert(new_grade >= 0);
                    assert(new_grade <= 100);
                    if (original_grade < 50 && new_grade >= 50) {
                        if (cs) {
                            s->cs_failure -= 1;
                            }
                            s->failure -= 1;
                        } else if (original_grade >= 50 && new_grade < 50) {
                            if (cs) {
                            s->cs_failure += 1;
                            }
                            s->failure += 1;
                        }
                    if (cs) {
                        s->cs_grades = s->cs_grades - original_grade + new_grade;
                        s->total_grades = s->total_grades - original_grade + new_grade;
                    } else {
                        s->total_grades = s->total_grades - original_grade + new_grade;
                    }
                  }

// cs_average(s) determines the average of all CS courses taken by s
// (or 0 if none taken)
int cs_average(const struct student *s) {
    assert(s);
    if (s->cs_courses == 0) {
        return 0;
    } 
    return s->cs_grades / s->cs_courses;
}

// cav(s) determines the Cumulative AVerage (CAV) of all courses taken by s
//  (or 0 if none taken)
int cav(const struct student *s) {
    assert(s);
    if (s->total_courses == 0) {
        return 0;
    } 
    return s->total_grades / s->total_courses;
}

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
void print_status (const struct student *s) {
    assert(s);
    int pass = s->total_courses - s->failure;
    if (s->total_courses == 0) {
        printf("NONE\n");
    } else if (s->failure > 4) {
        printf("FRW\n");
    } else if (s->failure > pass) {
        printf("FRW\n");
    } else if (s->cs_courses == 0) {
        printf("NOCS\n");
    } else if (cs_average(s) < 60 && cav(s) < 60) {
        printf("FLPP\n");
    } else if (cav(s) < 60) {
        printf("PRBO\n");
    } else if (cs_average(s) < 60) {
        printf("CNDP\n");
    } else if (cav(s) >= 60 && cav(s) < 80) {
        printf("GOOD\n");
    } else {
        printf("EXCL\n");
    }
}

// print_student(&s) prints the details of student s formatted exactly as 
//   illustrated by the following example:
//   "Student: 12345, CAV: 66, CS_AVG: 55, Failures: 1, Status: CNDP\n"
// effects: displays a message
void print_student(const struct student *s) {
    assert(s);
    printf("Student: %d, CAV: %d, CS_AVG: %d, Failures: %d, Status: ",
    s->id, cav(s), cs_average(s), s->failure);
    print_status(s);
}

// student_equals(&s1, &s2) determines if the two student records are
//   identical 
bool student_equals(const struct student *s1, const struct student *s2) {
    assert(s1);
    assert(s2);
    if (s1->cs_courses == s2->cs_courses && s1->cs_grades == s2->cs_grades &&
    s1->failure == s2->failure && s1->id == s2->id && s1->cs_failure == s2->cs_failure &&
    s1->total_courses == s2->total_courses && s1->total_grades == s2->total_grades) {
        return true;
    }
    return false;
}

// can_graduate(&s) determines if student s is eligible for graduation.
//   To graduate, a student must:
//     * have passed at least 40 courses, at least 15 of which must be CS
//     * have a status of GOOD or EXCL (see above)
bool can_graduate(const struct student *s) {
    assert(s);
    int pass = s->total_courses - s->failure;
    int cs_pass = s->cs_courses - s->cs_failure;
    if (s->failure <= 4 && s->failure <= pass && cs_average(s) >= 60
     && pass >= 40 && cs_pass >= 15 && cav(s) >= 60) {
        return true;
    }
    return false;
}