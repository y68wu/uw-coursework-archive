;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname nested) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 08
;;*******************************************
;;

;; ************* q4a **************

;; nested-listof-X-template: (nested-listof X) -> (listof Any)

(define (nested-listof-X-template lst)
  (cond
    [(empty? lst) ...]
    [(list? (first lst))
     (... (nested-listof-X-template (first lst))
          (nested-listof-X-template (rest lst)))]
    [else (... (first lst)
               ... (nested-listof-X-template (rest lst)))]))

;; ************* q4b **************

;; (nested-filter predicate nestlst) consumes a predicate function and a nested list (in that order)
;; The function produces a list that removes every element appears anywhere in the nested list
;; where the predicate function is false for that element.
;; nested-filter: (Any -> Bool) (nested-listof X) -> (nested-listof X)

;; Examples:

(check-expect (nested-filter even? (list 2 3 4 5 6)) (list 2 4 6))
(check-expect (nested-filter odd? (list 2 3 4 5 6)) (list 3 5))

(define (nested-filter predicate nestlst)
  (cond
    [(empty? nestlst) empty]
    [(list? (first nestlst))
     (cons (nested-filter predicate (first nestlst))
           (nested-filter predicate (rest nestlst)))]
    [(predicate (first nestlst))
     (cons (first nestlst) (nested-filter predicate (rest nestlst)))]
    [else (nested-filter predicate (rest nestlst))]))


;; Tests:

(check-expect (nested-filter odd? (list (list 3 4 5 6) (list 2 4 5 6) 3 7 3))
              (list (list 3 5) (list 5) 3 7 3))
(check-expect (nested-filter even? (list (list 3 4 5 6) (list 2 4 5 6) 3 7 3))
              (list (list 4 6) (list 2 4 6)))

;; ************* q4c **************

;; (ruthless symlst) consumes a nested list of symbols
;; The function produces an identical list except that all instances of 'ruth have been removed.
;; ruthless: (nested-listof Sym) -> (nested-listof Sym)

;; Examples:

(check-expect (ruthless empty) empty)
(check-expect (ruthless (list 'a 'b 'c 'd 'ruth)) (list 'a 'b 'c 'd))

(define (ruthless symlst)
  (local
    [(define (predicate e) (not (symbol=? e 'ruth)))]
    (nested-filter predicate symlst)))

;; Tests;

(check-expect (ruthless (list 'a (list 'ruth ' a 'b) 'b 'c 'd 'ruth))
              (list 'a (list 'a 'b) 'b 'c 'd))
(check-expect (ruthless (list (list 'a 'b 'c) (list 'ruth ' a 'b) 'b 'c 'd 'ruth))
              (list (list 'a 'b 'c) (list 'a 'b) 'b 'c 'd))


;; ************* q4d **************

;; (keep-between num1 num2 nestlst) consumes two numbers, a and b, and a nested list of numbers.
;; The function produces a nested list, keeping only the values between a and b inclusive.
;; keep-between: Num Num (nested-listof Num) -> (nested-listof Num)

;; Examples:

(check-expect (keep-between 1 2 empty) empty)
(check-expect (keep-between 1 10 (list 1 2 3 4 5 6 7 20 30)) (list 1 2 3 4 5 6 7))

(define (keep-between num1 num2 nestlst)
  (local
    [(define (between? e)
       (and (>= e num1) (<= e num2)))]
    (nested-filter between? nestlst)))


;; Tests:

(check-expect (keep-between 1 7 (list 9 2 (list 2 481 3 4 6 1) (list 1 45 2 6 7 ) 4 6 2))
              (list 2 (list 2 3 4 6 1) (list 1 2 6 7) 4 6 2))
(check-expect (keep-between 0 6 (list 4 6 67 3 (list 2 6 43 7) (list 2 6 4 6 7)))
              (list 4 6 3 (list 2 6) (list 2 6 4 6)))
(check-expect (keep-between 5 10 '(1 3 5 (7 9 10) (8 (3 4)) 8 15))
                       '(5 (7 9 10) (8 ()) 8))


;; ************* q4e **************

;; (nested-cleanup nestlst) consumes (nested-listof Any)
;; The function produces a list that removes all empty lists in the consumed (nested-listof Any)
;; And if there are no non-list elements anywhere in the list, the function produces false.
;; nested-cleanup: (nested-listof Any) -> (anyof (nested-listof Any) false)

;; Examples:

(check-expect (nested-cleanup '(1 () 2 () () 3)) '(1 2 3))
(check-expect (nested-cleanup '(1 (()()) 2 ((3 () (()))) )) '(1 2 ((3))))

(define (nested-cleanup nestlst)
  (cond
    [(empty? nestlst) false]
    [(not (list? (first nestlst)))
     (cond
       [(boolean? (nested-cleanup (rest nestlst)))
        (cons (first nestlst) empty)]
       [else (cons (first nestlst) (nested-cleanup (rest nestlst)))])]
    [(and (list? (first nestlst))
          (boolean? (nested-cleanup (first nestlst))))
     (nested-cleanup (rest nestlst))]
    [(boolean? (nested-cleanup (rest nestlst)))
     (cons (nested-cleanup (first nestlst)) empty)]
    [else
     (cons (nested-cleanup (first nestlst))
           (nested-cleanup (rest nestlst)))]))

;; Tests:

(check-expect (nested-cleanup '(()(()())(())())) false)
(check-expect (nested-cleanup '(3 () 4 () () 5)) '(3 4 5))