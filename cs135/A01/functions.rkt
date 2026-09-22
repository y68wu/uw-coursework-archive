;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname functions) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 01
;*******************************************
;;

; (a) An example from geometry (Euclidean distance):
(define (euclidean-distance x1 y1 x2 y2) (sqrt (+ (sqr (- x1 x2)) (sqr (- y1 y2)))))

; (b) An example from music (equal temperament):
(define (freq base-frequency interval) (* base-frequency (expt 2 (/ interval 12))))

; (c) Another example from geometry (surface area of right circular cone):
(define (cone-area r h) (* 2 r (+ pi (sqrt (+ (sqr h) (sqr r))))))

; (d) An example from physics (Lorentz Transformation Factor):
(define (B v c) (sqrt (- 1 (/ (sqr v) (sqr c)))))

; (e) An example from mathematics (the partition function):
(define (partition-size-approximation n) (* (/ 1 (* 4 n (sqrt 3))) (expt e (* pi (sqrt (/ (* 2 n) 3))))))

; (f) An example from finance (investment returns):
(define (future-value n p r t) (* p (expt (+ 1 (/ r n)) (* n t))))
