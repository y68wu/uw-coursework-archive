;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname range) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 04
;;*******************************************
;;

;; ********Q2a********

;; define the helper function for a num is between a and b (inclusive)
(define (between a b num)
  (cond
    [(and (>= num a) ;; a <= num <= b
          (<= num b)) true]
    [(and (>= num b)
          (<= num a)) true] ;; b <= num <=a
    [else false]))

;; (in-range a b numlist) consumes three parameters: a b and numlist  
;; This function produces the number of elements in the list that are between a and b (inclusive)
;; in-range: Num Num -> Int
;; (check-expect (in-range Num Num) Int)

;; Examples:
(define l1 (cons 8 (cons 1 (cons 4 (cons 3 (cons 2 (cons 7 empty)))))))
(check-expect (in-range 0 1 l1) 1)
(check-expect (in-range 1 4 l1) 4)

(define (in-range a b numlist)
  (cond
    [(empty? numlist) 0] ;; if the list is empty
    [(between a b (first numlist)) ;; if the first num is bwteen a and b
     (+ 1 (in-range a b (rest numlist)))] ;; +1 and recursion
    [else (in-range a b (rest numlist))])) ;; else recursion directly

;; Tests:
(check-expect (in-range 4 1 l1) 4)
(check-expect (in-range 2 8 l1) 5)

;; ********Q2b********

;; (max-num list) consumes a parameter: list
;; This function produces the maximum value of a non-empty number list
;; max-num: List -> Num
;; (check-expect (max-num List) Num)

;; Example:
;; (define l1 (cons 8 (cons 1 (cons 4 (cons 3 (cons 2 (cons 7 empty)))))))
(define l2 (cons 8 (cons -1 (cons 9 (cons 3 (cons 10 (cons 7 empty)))))))
(check-expect (max-num l1) 8)

(define (max-num list)
  (cond
    [(empty? (rest list)) (first list)]
    [(<= (first list) (first (rest list)))
     (max-num (rest list))]
    [else (max-num (cons (first list) (rest (rest list))))]))

;; Test:
(check-expect (max-num l2) 10)

;; (min-num list) consumes a parameter: list
;; This function produces the minimum value of a non-empty number list
;; min-num: List -> Num
;; (check-expect (min-num List) Num)

;; Example:
(check-expect (min-num l1) 1)

(define (min-num list)
  (cond
    [(empty? (rest list)) (first list)]
    [(>= (first list) (first (rest list)))
     (min-num (rest list))]
    [else (min-num (cons (first list) (rest (rest list))))]))

;; Tests:
(check-expect (min-num l2) -1)

;; (spread list) consumes a non-empty list of numbers 
;; The function produces the non-negative difference between the maximum element in the list
;; and minimum element of the list. 
;; spread: List -> Num
;; (check-expect (spread List) Num)

;; Example:
(define l3 (cons -2 (cons -12 (cons -9 (cons -3 (cons -10 (cons -7 empty)))))))
(define l4 (cons 3 (cons 3 (cons 3 empty))))
(check-expect (spread l1) 7)
(check-expect (spread l4) 0)

(define (spread list)
  (abs (- (max-num list) (min-num list)))) ;; Max - Min

;; Test:
;; Examples
(check-expect (spread l2) 11)
(check-expect (spread l3) 10)