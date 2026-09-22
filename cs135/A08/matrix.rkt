;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname matrix) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 08
;;*******************************************
;;


;; ************** q5a ****************

(define M (list (list -1 2 3) (list 4 5 6)
                (list 7 8.5 9)))

;; A Matrix is one of:
;; * empty
;; * (cons (listof Num) Matrix)

;; (matrix-apply funlst matrix) consumes a list of functions
;; (each with contract Num → Num, Num → Int,or Num → Nat) and a matrix,
;; The function produces a list of matrices.
;; The first matrix should be the result of applying the first function to each matrix element,
;; the second matrix should be the result of applying the second function to each matrix element,
;; and so on.
;; matrix-apply: (listof (Num -> (anyof Num Int Nat))) Matrix -> (listof Matrix)

;; Examples:

(check-expect (matrix-apply (list abs floor (lambda (x) (+ x 3))) '((7 4.5 -3.2)(-3 3 13)))
              (list (list (list 7 4.5 3.2)
                     (list 3 3 13))
               (list (list 7 4 -4)
                     (list -3 3 13))
               (list (list 10 7.5 -0.2)
                     (list 0 6 16))))
(check-expect (matrix-apply empty empty) empty)

(define (matrix-apply funlst matrix)
  (local
    ;; fun-apply-row: (Num -> (anyof Num Int Nat)) (listof Num) -> (listof Num)
    ;; The function produces the list of applying the function to each number in a row
    [(define (fun-apply-row function row)
       (cond
         [(empty? row) empty]
         [else (cons (function (first row))
                     (fun-apply-row function (rest row)))]))
     ;; fun-apply-matrix: (Num -> (anyof Num Int Nat)) Matrix -> Matrix
     ;; The function produces a Matrix that applying the function to each number in the Matrix
     (define (fun-apply-matrix function matrix)
       (cond
         [(empty? matrix) empty]
         [else (cons (fun-apply-row function (first matrix))
                     (fun-apply-matrix function (rest matrix)))]))]
    (cond
      [(empty? funlst) empty]
      [else (cons (fun-apply-matrix (first funlst) matrix)
                  (matrix-apply (rest funlst) matrix))])))

;; Tests:

(check-expect (matrix-apply empty '((1 2 3) (4 5 6) (7 8 9))) empty)
(check-expect (matrix-apply (list abs) '((-1 -2 -3) (-4 -5 -6) (-7 -8 -9)))
              (list (list (list 1 2 3) (list 4 5 6) (list 7 8 9))))


;; ************** q5b ****************

;; (scale-smallest matrix offset) consumes a non-empty matrix and a real number (the offset).
;; The function produces a second function that consumes a number,
;; multiplies that number by the smallest element of the matrix, and adds the offset.
;; scale-smallest: Matrix Num -> (Num -> Num)

;; Examples:

(check-expect ((scale-smallest '((7 4.5 3.2) (-3 3 13)) 2.4) 7) -18.6)
(check-expect ((scale-smallest '((7 4.5 3.2) (-3 3 13)) 2.4) -2.7) 10.5)

(define (scale-smallest matrix offset)
  (local
    ;; row-min: (listof Num) Num -> Num
    ;; The function produces the smallest number in a list
    [(define (row-min row acc)
       (cond
         [(empty? row) acc]
         [(<= (first row) acc) (row-min (rest row) (first row))]
         [else (row-min (rest row) acc)]))
     ;; matrix-min: Matrix -> Num
     ;; The function produces the smallest number in a list
     (define (matrix-min matrix)
       (cond
         [(empty? (rest matrix)) (row-min (first matrix) (first (first matrix)))]
         [else (min (row-min (first matrix) (first (first matrix)))
                    (matrix-min (rest matrix)))]))
     ;; apply-num: Num -> Num
     ;; The function produces the result that the number
     ;; multiplies the smallest element of the matrix, and adds the offset.
     (define (apply-num num)
       (+ offset (* num (matrix-min matrix))))]
    apply-num))

;; Tests:

(check-expect ((scale-smallest '((0 0 0) (0 0 0) (0 0 0)) 0) 0) 0)
(check-expect ((scale-smallest '((1 2 3)) 4) 1) 5)