;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname funabst) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 08
;;*******************************************
;;

;; ************* q2a ***************

;; (or-pred predicate lst) consumes a predicate (that consumes one argument) and a list,
;; The fucntion produces true if the application of the consumed predicate on any
;; element of the consumed list produces true, otherwise the function produces false.
;; Requires: If the consumed list is empty, the function should produce false.
;; or-pred: (Any -> Bool) (listof Any) -> Bool

;; Examples:

(check-expect (or-pred even? empty) false)
(check-expect (or-pred odd? (list 6 10 4)) false)

(define (or-pred predicate lst)
  (cond
    [(empty? lst) false]
    [(predicate (first lst)) true]
    [else (or-pred predicate (rest lst))]))

;; Tests:

(check-expect (or-pred string? (list 5 "wow")) true)
(check-expect (or-pred even? (list 2 4 6 8)) true)

;; ************* q2b ***************

;; (map2argfn funlist numlist) consumes a list of functions
;; (each of which takes two numbers as arguments) and a list containing two numbers.
;; The function produces the list of the results of applying each function in turn to the
;; given two numbers.
;; map2argfn: (listof (Num Num -> Any) (list Num Num) -> (listof Any)

;; Examples:

(check-expect (map2argfn (list + - * / list) (list 3 2)) (list 5 1 6 1.5 (list 3 2)))
(check-expect (map2argfn empty (list 3 2)) empty)

(define (map2argfn funlist numlist)
  (cond
    [(empty? funlist) empty]
    [else (cons ((first funlist) (first numlist) (second numlist))
                (map2argfn (rest funlist) numlist))]))

;; Tests

(check-expect (map2argfn empty (list 4 2)) empty)
(check-expect (map2argfn (list + - * /) (list 1 2)) (list 3 -1 2 0.5))

;; ************* q2c ***************

;; (arranged? funlist oplist) consumes a (list predicate-function binary-relational-operator) pair
;; and a list of values (operands). 
;; The function producestrue if the list of operands is empty or has one value
;; and applying the predicate on it produces true.
;; The function produces false if applying the predicate on any of the operands produces false.
;; The function produces true if applying the predicate on every operand produces true
;; and applying the binary relational operator on all consecutive elements of the list of operands
;; produces true, otherwise the function produces false.

;; arranged?: (list (Any -> Bool) (X X -> Bool)) (listof Any) -> Bool
;; requires: if binary-relational-operator is applied on any
;;           elements, then predicate-function produces true on
;;           elements of type X

;; Examples:

(check-expect (arranged? (list integer? <) (list)) true)
(check-expect (arranged? (list integer? >) (list 1)) true)

(define (arranged? funlist oplist)
  (cond
    [(empty? oplist) true]
    [(empty? (rest oplist)) ((first funlist) (first oplist))]
    [else (and ((first funlist) (first oplist))
               ((first funlist) (second oplist))
               ((second funlist) (first oplist) (second oplist))
               (arranged? funlist (rest oplist)))]))

;; Tests:

(check-expect (arranged? (list integer? >) (list 'red)) false)
(check-expect (arranged? (list string? >) (list "wow" 'red)) false)
(check-expect (arranged? (list string? string>?) (list "wow" "cs135" "amazing")) true)