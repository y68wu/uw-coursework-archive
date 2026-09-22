;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname bonus-a01) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 01
;*******************************************
;;

(define class-participation-grade-rate (/ 5 100))
(define midterm-grade-rate (/ 20 100))
(define final-exam-grade-rate (/ 45 100))
(define overall-assignments-grade-rate (/ 30 100))

(define standard-rate 50)
(define min-grade 46)

(define (exam-component
         midterm-grade
         final-exam-grade)
  (+ (* midterm-grade midterm-grade-rate)
     (* final-exam-grade final-exam-grade-rate)))

(define (weighted-exam-component
         midterm-grade
         final-exam-grade)
  (* (exam-component midterm-grade final-exam-grade)
     (+ midterm-grade-rate final-exam-grade-rate)))

(define (assignment-component
         overall-assignments-grade)
  (* overall-assignments-grade overall-assignments-grade-rate))

(define (class-participation-component class-participation-grade)
  (* class-participation-grade class-participation-grade-rate))

(define (regular-final-cs135-grade
         exam-component
         assignment-component
         class-participation-component)
  (+ exam-component
     assignment-component
     class-participation-component))

(define (specific-condition
         assignment-grade
         midterm-grade
         final-exam-grade)
  (min (min assignment-grade
            (weighted-exam-component midterm-grade final-exam-grade))
       standard-rate))

(define (final-grade
         assignment-grade
         midterm-grade
         final-exam-grade)
  (min (regular-final-cs135-grade
        assignment-grade
        midterm-grade
        final-exam-grade)
       min-grade))







