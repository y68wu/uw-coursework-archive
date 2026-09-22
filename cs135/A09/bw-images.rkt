;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname bw-images) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 09
;;*******************************************
;;

(define image-L '((1 0 0 0)
                  (1 0 0 0)
                  (1 0 0 0)
                  (1 1 1 1)))

(define image-L-reflect-x '((1 1 1 1)
                            (1 0 0 0)
                            (1 0 0 0)
                            (1 0 0 0)))

(define image-L-reflect-y '((0 0 0 1)
                            (0 0 0 1)
                            (0 0 0 1)
                            (1 1 1 1)))

(define image-L-inverted '((0 1 1 1)
                           (0 1 1 1)
                           (0 1 1 1)
                           (0 0 0 0)))

;; ************** q3a ***************

;; (invert image) consumes a 2D-Image image
;; The function produces a 2D-Image that is the inverted
;; invert: 2D-Image -> 2D-Image

;; Example:

(check-expect (invert empty) empty)

(define (invert image)
  (map (lambda (y)
         (map (lambda (x)
                (cond
                  [(= x 0) 1]
                  [(= x 1) 0])) y)) image))

;; Test:

(check-expect (invert image-L) image-L-inverted)

;; ************** q3b ***************

;; (reflect-x-axis image) consumes a 2D-Image
;; The function produces a 2D-Image that represents the reflection of the 2D-Image across the x-axis.
;; reflect-x-axis: 2D-Image -> 2D-Image

;; Example:

(check-expect (reflect-x-axis empty) empty)

(define (reflect-x-axis image)
  (reverse image))

;; Test:

(check-expect (reflect-x-axis image-L) image-L-reflect-x)

;; ************** q3c ***************

;; (reflect-y-axis image) consumes a 2D-Image
;; The function produces a 2D-Image that represents the reflection of the 2D-Image across the y-axis.
;; reflect-y-axis: 2D-Image -> 2D-Image

;; Example:

(check-expect (reflect-y-axis empty) empty)

(define (reflect-y-axis image)
  (map reverse image))

;; Test:

(check-expect (reflect-y-axis image-L) image-L-reflect-y)

;; ************** q3d ***************

;; (transpose image) consumes a 2D-Image
;; The function produces a 2D-Image that represents the transposed image.
;; transpose: 2D-Image -> 2D-Image

;; Example:

(check-expect (transpose image-L)
              '((1 1 1 1)
                (0 0 0 1)
                (0 0 0 1)
                (0 0 0 1)))

(check-expect (transpose empty) empty)

(define (transpose image)
  (reverse (foldr
            (lambda (row new-image)
              (cond
                [(empty? new-image) (map list row)]
                [else (map cons row new-image)]))
            empty
            (map reverse image))))

;; Test:

(check-expect (transpose '((1 0 0 1 1))) '((1) (0) (0) (1) (1)))

(check-expect (transpose '((1 2 3 4) (5 6 7 8) (9 10 11 12) (13 14 15 16)))
              '((1 5 9 13) (2 6 10 14) (3 7 11 15) (4 8 12 16)))