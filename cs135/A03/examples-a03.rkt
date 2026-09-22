;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname examples-a03) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 03
;*******************************************
;;

;; Examples for waterloo2-poker

; ********** a ************
;(define card-c1 (cons 2 (cons 'S empty)))
;(define card-c2 (cons 3 (cons 'H empty)))
;(define card-c3 (cons 4 (cons 'D empty)))
;(check-expect (ordinality card-c1) 2)
;(check-expect (ordinality card-c2) 3)
;(check-expect (ordinality card-c3) 4)

; ********** b ************
;; Examples:
;(define hand1 (cons (cons 4 (cons 'S empty)) (cons (cons 4 (cons 'D empty)) empty)))
;(check-expect (strength hand1) 3)

; ********** c ************
;; Examples:
;;(define hand1 (cons (cons 4 (cons 'S empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand2 (cons (cons 3 (cons 'D empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand3 (cons (cons 4 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))
;;(define hand4 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'S empty)) empty)))
;;(define hand5 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))

;(check-expect (hand<? hand1 hand2) true)
;(check-expect (hand<? hand3 hand4) false)

; ********** d ************
;; Examples
;;(define hand1 (cons (cons 4 (cons 'S empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand2 (cons (cons 3 (cons 'D empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand3 (cons (cons 4 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))
;;(define hand4 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'S empty)) empty)))
;;(define hand5 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))

;(check-expect (winner hand1 hand2) 'hand2)
;(check-expect (winner hand3 hand4) 'hand1)

;; Examples for collide

;; *********** (a) *************
;; Examples:
;define p1 (cons 0 (cons 1 (cons 2 empty))))
;(define p2 (cons 1 (cons 3 (cons 4 empty))))
;(define p3 (cons 2 (cons 5 (cons 6 empty))))
;(check-expect (distance-between-points p1 p2) 3)
;(check-expect (distance-between-points p2 p3) 3)

;; *********** (b) *************
;; Examples:
; (define p1 (cons 0 (cons 1 (cons 2 empty))))
; (define p2 (cons 1 (cons 3 (cons 4 empty))))
; (define p3 (cons 2 (cons 5 (cons 6 empty))))

;(define sphere1 (cons p1 (cons 3 empty)))
;(define sphere2 (cons p1 (cons 3 empty)))
;(check-expect (point-in-sphere? p1 sphere1) true)
;(check-expect (point-in-sphere? p2 sphere1) false)

;; *********** (c) ************

;; Examples:
;(define sphere3 (cons p2 (cons 5 empty)))
;(define sphere4 (cons p2 (cons 2 empty)))
;(check-expect (collide? sphere1 sphere2) true)
;(check-expect (collide? sphere1 sphere4) false)