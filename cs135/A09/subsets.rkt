;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname subsets) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 09
;;*******************************************
;;

;; ************ q4a *************

(define (subsets1 lst)
  (foldr (lambda (item acc)
           (append acc
                   (map (lambda (sub) (cons item sub)) acc)))
         '(())
         lst))

;; ************ q4b *************

(define (subsets2 lst)
  (foldr (lambda (item acc)
           (append acc
                   (map (lambda (sub) (cons item sub)) acc)))
         '(())
         lst))

;; ************ q4c *************

(define subsets3
  (lambda (lst)
    (cond
      [(empty? lst) '(())]
      [else (append
             (subsets3 (rest lst))
             (map (lambda (subset)
                    (cons (first lst) subset))
                  (subsets3 (rest lst))))])))