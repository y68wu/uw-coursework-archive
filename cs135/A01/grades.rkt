;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname grades) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
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

(define minimum-grade 60)

(define (final-cs135-grade
         class-participation-grade
         midterm-grade
         final-exam-grade
         overall-assignments-grade)
  (+ (* class-participation-grade class-participation-grade-rate)
     (* midterm-grade midterm-grade-rate)
     (* final-exam-grade final-exam-grade-rate)
     (* overall-assignments-grade overall-assignments-grade-rate)))

(define (cs135-final-exam-grade-needed
         class-participation-grade
         midterm-grade
         overall-assignments-grade)
  (/ (- minimum-grade
        (+ (* class-participation-grade class-participation-grade-rate)
           (* midterm-grade midterm-grade-rate)
           (* overall-assignments-grade overall-assignments-grade-rate)))
     final-exam-grade-rate))

