;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname bonus-a08) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 08
;;*******************************************
;;


;; ************* a ***************

;; (my-compose f g) consumes two functions f and g in that order,
;; The function produces a function that when applied to an argument x gives the same result
;; as if g is applied to x and then f is applied to the result
;; my-compose: func func -> func

(define (double x) (* x 2))
(define (add-three x) (+ x 3))

;; Examples:

(check-expect ((my-compose double add-three) -2) 2)
(check-expect ((my-compose double add-three) 10) 26)

(define (my-compose f g)
  (lambda (x)
    (f (g x))))

;; Tests:

(check-expect ((my-compose double add-three) 4) 14)
(check-expect ((my-compose double add-three) 0) 6)

;; ************* b ***************

;; (curry f) consumes one two-argument function f
;; The function produces a one-argument function that when applied to an argument x
;; produces another function that, if applied to an argument y, gives the same result
;; as if f had been applied to the two arguments x and y.
;; curry: func -> func

(define (add x y) (+ x y))

;; Examples:

(check-expect (((curry add) 0) 0) 0)
(check-expect (((curry add) -3) -2) -5)


(define (curry f)
  (lambda (x)
    (lambda (y)
      (f x y))))

;; Tests:

(check-expect (((curry add) 5) 3) 8)
(check-expect (((curry add) 10) -2) 8)

;; ************* c ***************

;; (uncurry f) produces the opposite of curry, in the sense that for any two-argument function f
;; (uncurry (curry f)) is functionally equivalent to f.
;; uncurry: func -> func

;; Examples:


(check-expect (((curry add) 5) 3) 8)
(check-expect (((curry add) 10) -2) 8)

(define (uncurry f)
  (lambda (x y)
    ((f x) y)))

;; Tests:

(check-expect (((curry add) 0) 0) 0)
(check-expect (((curry add) -3) -2) -5)

;; ************* d ***************

;; Examples:

(check-expect (eat-apples (list 'apple 'banana 'apple 'cherry)) (list 'banana 'cherry))
(check-expect (eat-apples (list 'banana 'cherry)) (list 'banana 'cherry))

(define (eat-apples lst)
  (local
    [(define (not-apple? x)
       (not (equal? x 'apple)))]
    (filter not-apple? lst)))

;; Tests:

(check-expect (eat-apples (list 'apple 'apple 'apple)) empty)
(check-expect (eat-apples empty) empty)