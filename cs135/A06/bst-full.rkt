;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname bst-full) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 06
;;*******************************************
;;

;; *********** Q3a ************

(define-struct node (key left right))
;; A Node is a (make-node Nat BST BST)
;; requires: key > every key in left BST ;; key < every key in right BST
     ;; A Binary Search Tree (BST) is one of:
     ;; * empty
     ;; * Node

(define node1 (make-node 5 (make-node 1 empty empty) empty))
(define node2 (make-node 5 (make-node 4 empty empty) (make-node 6 empty empty)))
(define node3 (make-node 5 (make-node 4 empty empty)
                         (make-node 6 (make-node 3 empty empty) empty)))

;; (full? bst) that consumes a binary search tree
;; The function produces true iff the given binary search tree is full.
;; The empty tree, paradoxically, is considered full.
;; full?: BST -> Bool

;; Examples:

(check-expect (full? node1) false)
(check-expect (full? node2) true)

(define (full? bst)
  (cond
    [(empty? bst) true]
    [(and (empty? (node-left bst))
          (empty? (node-right bst))) true]
    [(or (empty? (node-left bst))
         (empty? (node-right bst))) false]
    [else (and (full? (node-left bst))
               (full? (node-right bst)))]))

;; Tests:

(check-expect (full? empty) true)
(check-expect (full? node3) false)


;; *********** Q3b ************

(define node4 (make-node 5 (make-node 4 (make-node 3 (make-node 2 empty empty) empty) empty) empty))
(define node5 (make-node 5 (make-node 4 (make-node 3 (make-node 2 (make-node 1 empty empty)
                                                                (make-node 3 empty empty))
                                                   empty) empty) empty))
(define node6 (make-node 5 empty (make-node 6 (make-node 4 (make-node 3 empty empty) empty) empty)))
(define node7 (make-node 5 empty (make-node 6 empty (make-node 7 empty (make-node 8 empty empty)))))

;; Examples:
(check-expect (left-single-path? node4) true)
(check-expect (left-single-path? node5) false)

;; determine if there exist a left-single-path

(define (left-single-path? bst)
  (cond
    [(and (empty? (node-left bst)) (empty? (node-right bst))) true]
    [else (and (empty? (node-right bst))
          (left-single-path? (node-left bst)))]))

;; Tests:

(check-expect (left-single-path? node6) false)
(check-expect (left-single-path? node7) false)

;; determine if there exist a right-single-path

;; Examples:
(check-expect (right-single-path? node4) false)
(check-expect (right-single-path? node5) false)

(define (right-single-path? bst)
  (cond
    [(and (empty? (node-left bst)) (empty? (node-right bst))) true]
    [else (and (empty? (node-left bst))
          (right-single-path? (node-right bst)))]))

;; Tests:

(check-expect (right-single-path? node6) false)
(check-expect (right-single-path? node7) true)


;; (single-path? bst) consumes a binary search tree
;; The function produces true iff the given binary search tree looks like a single path.
;; A binary search tree looks like a single path iff all children are left children,
;; or all children are right children.
;; require: the given tree has at least two nodes.
;; single-path?: BST -> Bool

;; Examples:

(check-expect (single-path? node4) true)
(check-expect (single-path? node5) false)

(define (single-path? bst)
  (or (left-single-path? bst) (right-single-path? bst)))

;; Tests:

(check-expect (single-path? node6) false)
(check-expect (single-path? node7) true)