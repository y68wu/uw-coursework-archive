;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname bonus-a06) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 06
;;*******************************************
;;

;; (list-xor-dups lst1 lst2) consumes two lists of integers in non-decreasing order.
;; That means there may be duplicates.
;; The function produces a list sorted in increasing order consisting of all the integers
;; that appear on one of the lists, but not both, with duplicates removed.
;; list-xor-dups: (listof Num) (listof Num) -> (listof Num)

;; Example:

(check-expect (list-xor-dups (list 4 4 4 5 5 6 6) (list 1 2 3 3 3 4 4 4 7)) (list 1 2 3 5 6 7))


(define (list-xor-dups lst1 lst2)
  (cond
    [(empty? lst1) lst2]
    [(empty? lst2) lst1]
    [(= (first lst1) (first lst2)) (list-xor-dups (rest lst1) (rest lst2))]
    [(and (not (empty? (rest lst1)))
          (= (first lst1) (first (rest lst1)))
          (< (first lst1) (first lst2))) (list-xor-dups (rest lst1) lst2)]
    [(< (first lst1) (first lst2)) (cons (first lst1) (list-xor-dups (rest lst1) lst2))]
    [(and (not (empty? (rest lst2)))
          (= (first lst2) (first (rest lst2)))
          (< (first lst2) (first lst1))) (list-xor-dups lst1 (rest lst2))]
    [(< (first lst2) (first lst1)) (cons (first lst2) (list-xor-dups lst1 (rest lst2)))]
    [else (list-xor-dups (rest lst1) lst2)]))

;; Test:

(check-expect (list-xor-dups (list 1 2 3 4 4 4 5 7 7 8) (list 1 2 2 3 5 5 7 8 9)) (list 2 4 5 7 9))