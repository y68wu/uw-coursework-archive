;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname tree-pred) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 08
;;*******************************************
;;

(define-struct node (key left right))

(define t (make-node 5
(make-node 10 empty empty)
                          (make-node 15
                                     (make-node 20 empty empty)
                                     (make-node 33 empty empty))))

;; (tree-pred predicate) consumes a one-argument predicate (that consumes a Nat)
;; The function produces a function. That function will consume a binary tree and produce true
;; if the predicate produces true for every value in the tree and false otherwise.
;; Require: If the tree is empty, the produced function should produce true.
;; tree-pred: (Any -> Bool) -> (BT -> Bool)

;; Examples:

(check-expect ((tree-pred even?) t) false)
(check-expect ((tree-pred positive?) t) true)

(define (tree-pred predicate)
  (local
    [(define (judge bt)
       (cond
         [(node? bt)
          (and (predicate (node-key bt))
               (judge (node-left bt))
               (judge (node-right bt)))]
         [(empty? bt) true]))]
    judge))

;; Tests:

(check-expect ((tree-pred odd?) t) false)
(check-expect ((tree-pred negative?) t) false)