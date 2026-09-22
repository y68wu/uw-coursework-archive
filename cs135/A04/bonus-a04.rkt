;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname bonus-a04) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 04
;;*******************************************
;;

(require "a04bonuslib.rkt")

;; (sum-of-digits num) consumes a parameter num (Nat)
;; The function produces the sum of all the digits of a Nat number (num)
;; sum-of-digits: Nat -> Int
;; (check-expect (sum-of-digits Nat) Int)

;; Examples:
(check-expect (sum-of-digits 12345) 15)
(check-expect (sum-of-digits 22222) 10)

(define (sum-of-digits num)
  (cond
    [(and (= (other-digits num) 0)
          (= (last-digit num) 0)) 0]
    [else (+ (last-digit num)
             (sum-of-digits (other-digits num)))]))

;; Tests:
(check-expect (sum-of-digits 33303) 12)
(check-expect (sum-of-digits 3030303) 12)

(define (mod3-zero? num)
  (cond
    [(= num 0) true]
    [else (mod3-zero? (- num 3))]))

;; (>0mod3-zero? num) consumes a parameter num (Nat)(num > 0)
;; The function produces true if the Nat num (num > 0) mod 3 is zero and false otherwise.
;; >0mod3-zero?: Nat -> Bool
;; (check-expect (>0mod3-zero? Nat) Bool)

;; Example:
(check-expect (>0mod3-zero? 123) true)

(define (>0mod3-zero? num)
  (cond
    [(= num 0) true]
    [(< num 0) false]
    [else (>0mod3-zero? (- num 3))]))

;; Test:
(check-expect (>0mod3-zero? 124) false)

;; (<0mod3-zero? num) consumes a parameter num (Nat)(num < 0)
;; The function produces true if the Nat num (num < 0) mod 3 is zero and false otherwise.
;; <0mod3-zero?: Nat -> Bool
;; (check-expect (<0mod3-zero? Nat) Bool)

;; Example:
(check-expect (<0mod3-zero? -123) true)

(define (<0mod3-zero? num)
  (cond
    [(= num 0) true]
    [(> num 0) false]
    [else (<0mod3-zero? (+ num 3))]))

;; Test:
(check-expect (<0mod3-zero? -124) false)

;; (div-by-3-alt? num) consumes a parameter num (Nat)
;; The function produces true if the Nat (num) divisible by 3 and false otherwise.
;; div-by-3-alt?: Nat -> Bool
;; (check-expect (div-by-3-alt? Nat) Bool)

;; Examples:
(check-expect (div-by-3-alt? 369) true)
(check-expect (div-by-3-alt? 369126) true)
(check-expect (div-by-3-alt? 169126) false)

(define (div-by-3-alt? num)
  (cond
    [(= num 0) true] ;; num = 0, 0 mod 3 = 0
    [(> num 0) (>0mod3-zero? (sum-of-digits num))] ;; num > 0
    [(< num 0) (<0mod3-zero? (sum-of-digits num))])) ;; num < 0

;; Tests:
(check-expect (div-by-3-alt? 60120) true)
(check-expect (div-by-3-alt? 0) true)
(check-expect (div-by-3-alt? 24866) false)