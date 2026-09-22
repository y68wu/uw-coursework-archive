;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname median) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 02
;*******************************************
;;

;; (median-of-3-simple n1 n2 n2) consumes three numbers
;; (median-of-3-simple n1 n2 n2) produces the median of three numbers n1 n2 n3
;; median-of-3-simple: Num Num Num -> Num
;; (check-expect (median-of-3-simple Num Num Num) Num)
;;examples:
(check-expect (median-of-3-simple 1 2 3) 2)
(check-expect (median-of-3-simple 4 6 5) 5)

(define (median-of-3-simple n1 n2 n3)
  (cond
    [(or (<= n2 n1 n3) (<= n3 n1 n2)) n1] ;; n1 is between n2 and n3
    [(or (<= n1 n2 n3) (<= n3 n2 n1)) n2] ;; n2 is between n1 and n3
    [else n3])) ;; ;; n3 is between n1 and n2

;;Tested:
(check-expect (median-of-3-simple 2 4 3) 3)
(check-expect (median-of-3-simple 5 1 6) 5)
(check-expect (median-of-3-simple 6 -3 0) 0)
(check-expect (median-of-3-simple 2 -3 -2) -2)