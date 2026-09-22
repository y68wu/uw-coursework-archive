;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname recursion) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 06
;;*******************************************
;;


;; ************ Q1a *************

;; (in-range/acc a b lst acc) consumes three parameters a b lst acc
;; The function produces the number of elements in the list that are between a and b (inclusive) plus
;; the accumulater acc
;; in-range/acc: Num Num (listof Num) Int -> Int

;; Example:

(check-expect (in-range/acc 3 4 (list 3 3.23 4 5) 0) 3)

(define (in-range/acc a b lst acc)
  (cond
    [(empty? lst) acc]
    [(and (>= (first lst) a) (<= (first lst) b))
     (in-range/acc a b (rest lst) (add1 acc))]
    [(and (>= (first lst) b) (<= (first lst) a))
     (in-range/acc a b (rest lst) (add1 acc))]
    [else (in-range/acc a b (rest lst) acc)]))

;; Test:

(check-expect (in-range/acc 3 4 (list 3 3.23 4 5) 1) 4)

;; (in-range/acc a b lst acc) consumes two parameters a b numlst
;; The function produces the number of elements in the list that are between a and b (inclusive)
;; in-range: Num Num (listof Num) -> Int

;; Example:

(check-expect (in-range 3 4 (list 3 3.23 4 5)) 3)

(define (in-range a b numlst)
  (in-range/acc a b numlst 0))

;; Test:

(check-expect (in-range 3 8 (list 1 2 39 593 1 34 56 7 4 89 6 8)) 4)
(check-expect (in-range 3.5 4.5 (list 3 3.23 4 5)) 1)

;; ************ Q1b *************

;; (spread/acc min max lst) consumes three parameters min max lst
;; The function produces the non-negative difference between the maximum element in the list and max,
;; and minimum element of the list and min
;; spread/acc: Num Num (listof Num) -> Num

;; Example:

(check-expect (spread/acc 1 6 (list 2 3 4 5 5 8)) 7)
(check-expect (spread/acc 1 9 (list 2 3 4 5 5 8)) 8)

(define (spread/acc min max lst)
  (cond
    [(empty? lst) (- max min)]
    [(<= (first lst) min) (spread/acc (first lst) max (rest lst))]
    [(>= (first lst) max) (spread/acc min (first lst) (rest lst))]
    [else (spread/acc min max (rest lst))]))

;;Tests:

(check-expect (spread/acc 2 2 (list 2 3 4 5 5 8)) 6)
(check-expect (spread/acc 3 9 (list 2 3 4 5 5 8)) 7)

;; (spread numlst) consumes a non-empty list of numbers
;; The function produces the non-negative difference between the maximum element in the list
;; and minimum element of the list.
;; If the maximum element is the same as the minimum element, spread produces zero.
;; spread: (listof Num) -> Num

;; Example:
(check-expect (spread (list 2 3 4 5 5 8)) 6)

(define (spread numlst)
  (spread/acc (first numlst) (first numlst) numlst))

;; Tests:
(check-expect (spread (list 2 2 2 2)) 0)
(check-expect (spread (list -2.3 3 4 5 5 8)) 10.3)

;; ************ Q1c *************

(define (smallest-first/acc numlst acc acclst)
  (cond
    [(empty? numlst) acclst]
    [(<= (first numlst) acc)
     (smallest-first/acc (rest numlst) (first numlst) (cons (first numlst) acclst))]
    [else (smallest-first/acc (rest numlst) acc (append acclst (list (first numlst))))]))

;; Tests:

(check-expect (smallest-first/acc (list 3 6 4 1 8) 3 empty) (list 1 3 6 4 8))
(check-expect (smallest-first/acc (list 2) 3 empty) (list 2))

;; (smallest-first lst) consumes a non-empty list of numbers
;; The function produces that same list but with a smallest element at the beginning of the list.
;; The order of the remaining elements is not specified.
;; smallest-first: (listof Num) -> (listof Num)

;; Example:

(check-expect (smallest-first (list 3 6 4 1 8)) (list 1 3 6 4 8))

(define (smallest-first lst)
  (smallest-first/acc lst (first lst) empty))

;; Tests:

(check-expect (smallest-first (list 2)) (list 2))
(check-expect (smallest-first (list 2 0)) (list 0 2))

;; (sel-sort/sf numlst) consumes a non-empty list of numbers where the smallest element in
;; the list is the first one (“sf” stands for “smallest first”) and produces a sorted list.
;; sel-sort/sf: (listof Num) -> (listof Num)

;; Example:

(check-expect (sel-sort/sf (list 1 5 2 7 3 9)) (list 1 2 3 5 7 9))

(define (sel-sort/sf numlst)
  (cond
    [(empty? (rest numlst)) (cons (first numlst) empty)]
    [else (cons (first numlst) (sel-sort/sf (smallest-first (rest numlst))))]))

;; Tests:
(check-expect (sel-sort/sf (list 1 5 2 7 3 9 11 15)) (list 1 2 3 5 7 9 11 15))
(check-expect (sel-sort/sf (list 2)) (list 2))

;; (sel-sort numlst) consumes a list of numbers
;; The function produces the result of sorting the list of numbers into non-decreasing
;; order using an algorithm known as “selection sort”
;; sel-sort: (listof Num) -> (listof Num)

;; Examples:

(check-expect (sel-sort (list 1 5 2 7 3 9 11 15)) (list 1 2 3 5 7 9 11 15))
(check-expect (sel-sort (list 2)) (list 2))

(define (sel-sort numlst)
  (cond
    [(empty? numlst) empty]
    [else (sel-sort/sf (smallest-first numlst))]))

;; Tests:

(check-expect (sel-sort (list 11 5 2 7 3 9 1 15)) (list 1 2 3 5 7 9 11 15))
(check-expect (sel-sort empty) empty)