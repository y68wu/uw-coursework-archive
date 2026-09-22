;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname div-by-3) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 04
;;*******************************************
;;

;;********Q4a********

;; The definition for natural numbers is that 0, 1, and 2 are all natural numbers
;; and any number that is three larger than a natural number is also a natural number.

;; Data definition for natural numbers (Nat3):
;; Nat3 is one of:
;; - 0
;; - 1
;; - 2
;; - (+ 3 n)  ; where n is a Nat3

;; (nat3-template n) consumes a parameter n
;; This function produces true if n is a natural number and false otherwise
;; nat3-template: Nat3 -> Bool

(define (nat3-template n)
  (cond
    [(= 0 n) ...] ;; Base case: n = 0
    [(= 1 n) ...] ;; Base case: n = 1
    [(= 2 n) ...] ;; Base case: n = 2
    [else (... n
               (nat3-template (- n 3)))])) ;;; Recursion

;;********Q4b********

;; (div-by-3? n) consumes a parameter n 
;; This function produces true if the Nat3 n is divisible by 3 and false otherwise.
;; div-by-3?: Nat3 -> Bool
;; (check-expect (div-by-3? Nat3) bool)

;; Examples:
(check-expect (div-by-3? 123) true)
(check-expect (div-by-3? 5) false)
(check-expect (div-by-3? 4) false)

(define (div-by-3? n)
  (cond
    [(= n 0) true] ;; base case: 0 is divisible by 3 (0+3, 0+3+3, ...)
    [(= n 1) false] ;; base case: 1 is not divisible by 3 (1+3, 1+3+3, ...)
    [(= n 2) false] ;; base case: 2 is not divisible by 3 (2+3, 2+3+3, ...)
    [else (div-by-3? (- n 3))])) ;; recursion

;; Tests:
(check-expect (div-by-3? 12) true)
(check-expect (div-by-3? 13) false)
(check-expect (div-by-3? 14) false)
(check-expect (div-by-3? 0) true)
