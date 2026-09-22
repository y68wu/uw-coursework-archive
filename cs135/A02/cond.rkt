;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname cond) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 02
;*******************************************
;;

;;
;(a) simplify
;;(define (q3a p1? p2?) (cond
;[p2? (cond [p1? 'left] [else 'down])]
;[else (cond [p1? 'up]
;[else 'right])]))
;;

(define (q3a p1? p2?)
  (cond
    [(and p1? p2?) 'left]
    [p2? 'down]
    [p1? 'up]
    [else 'right]))

;;
;(b) simplify
;;(define (q3b p1? p2?) (cond [p1? (cond
;[p2? (cond
;[p1? 'up]
;[p2? 'down]
;[else 'right])] [else (cond
;[p2? 'down]
;[else 'up])])] [(and p1? false) (cond [p2? 'left]
;[else 'right] 
;[else 'down]))
;;

(define (q3b p1? p2?)
  (cond
    [(and p1? p2?) 'up]
    [(and p1? (not p2?)) 'up]
    [else 'down]))

;;
;(c) simplify
;;(define (q3c p1? p2?)
;(cond [(cond [p1? p2?]
;(q3c p1? p2?)
;[else true]) 'up] [else 'down]))
;;

(define (q3c p1? p2?)
  (cond
    [(and p1? (not p2?)) 'down]
    [else 'up]))