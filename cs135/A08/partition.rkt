;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname partition) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 08
;;*******************************************
;;

;; ************ q1b ************

;; (partition predicate lst) consumes a predicate and a list.
;; The function produces a two element list, (list X Y),
;; where X is a list of those items in the consumed list that satisfy the predicate
;; and Y is a list of those items that don’t satisfy the predicate.
;; Requires: The order of items in each list must be the same as the original list.
;; partition: (Any -> Bool) (listof Any) -> (listof (listof Any) (listof Any))

;; Examples:

(check-expect (partition even? (list 2 3 4 5 6 7 8))
              (list (list 2 4 6 8) (list 3 5 7)))
(check-expect (partition odd? (list 1 2 4 3 5 6 7 8 9 11))
              (list (list 1 3 5 7 9 11) (list 2 4 6 8)))

(define (partition predicate lst)
  (local
    ;; match consumes a list, a predicate and two accumulate lists
    ;; match produces a list of length two
    ;; match: (listof Any) predicate (listof Any) (listof Any) -> (listof Any) 
    [(define (match lst predicate acc1 acc2)
     (cond
       [(empty? lst) (list (reverse acc1) (reverse acc2))]
       [(predicate (first lst))
        (match (rest lst) predicate (cons (first lst) acc1) acc2)]
       [else
        (match (rest lst) predicate acc1 (cons (first lst) acc2))]))]
    (match lst predicate empty empty)))

;; Tests:

(check-expect (partition odd? (list 3 5 1 43 2 5 56 7))
              (list (list 3 5 1 43 5 7) (list 2 56)))
(check-expect (partition even? (list 2 4 3 2 5 6 7 9))
              (list (list 2 4 2 6) (list 3 5 7 9)))