;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname bonus-a03) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 03
;*******************************************
;;

;; box: (cons xmin (cons xmax (cons ymin (cons ymax empty))))

;; define xmin of a box
(define (xmin box)
  (first box))

;; define xmax of a box
(define (xmax box)
  (first (rest box)))

;; define ymin of a box
(define (ymin box)
  (first (rest (rest box))))

;; define ymax of a box
(define (ymax box)
  (first (rest (rest (rest box)))))

;; (overlap-area box1 box2) consumes two parameter: box1 box2
;; The function produces the size of the overlapping region.
;; If the boxes do not overlap, then 0 should be produced.
;; A negative value should never be produced.
;; overlap-area: box box -> Num
;; (check-expect (overlap-area box box) Num)

;; Examples:

;; box: (cons xmin (cons xmax (cons ymin (cons ymax empty))))

(define box1 (cons 1 (cons 2 (cons 3 (cons 4 empty)))))
(define box2 (cons 3 (cons 4 (cons 5 (cons 6 empty)))))
(define box3 (cons 2 (cons 5 (cons 4 (cons 8 empty)))))
(check-expect (overlap-area box1 box2) 0)
(check-expect (overlap-area box2 box3) 4)


(define (overlap-area
         box1
         box2)
  (cond
    [(and (>= (xmin box1) (xmin box2)) ;; xmin box1 >= xmin box2
          (<= (xmin box1) (xmax box2)) ;; xmin box1 <= xmax box2
          (>= (ymax box1) (ymin box2)) ;; ymax box1 >= ymin box2
          (<= (ymax box1) (ymax box2))) ;; ymax box1 <= ymax box2
     (* (- (xmax box2) (xmin box1)) ;; (xmax box2 - xmin box1) * (ymax box1 - ymin box2)
        (- (ymax box1) (ymin box2)))]
    [(and (>= (xmin box1) (xmin box2)) ;; xmin box1 >= xmin box2
          (<= (xmin box1) (xmax box2)) ;; xmin box1 <= xmax box2
          (>= (ymin box1) (ymin box2)) ;; ymin box1 >= ymin box2
          (<= (ymin box1) (ymax box2))) ;; ymin box1 <= ymax box2
     (* (- (xmax box2) (xmin box1)) ;; (xmax box2 - xmin box1) * (ymax box2 - ymin box1)
        (- (ymax box2) (ymin box1)))]
    [(and (>= (xmax box1) (xmin box2)) ;; xmax box1 >= xmin box2
          (<= (xmax box1) (xmax box2)) ;; xmax box1 <= xmax box2
          (>= (ymax box1) (ymin box2)) ;; ymax box1 >= ymin box2
          (<= (ymax box1) (ymax box2))) ;; ymax box1 <= ymax box2
     (* (- (xmax box1) (xmin box2)) ;; (xmax box1 - xmin box2) * (ymax box1 - ymin box2)
        (- (ymax box1) (ymin box2)))]
    [(and (>= (xmax box1) (xmin box2)) ;; xmax box1 >= xmin box2
          (<= (xmax box1) (xmax box2)) ;; xmax box1 <= xmax box2
          (>= (ymin box1) (ymin box2)) ;; ymin box1 >= ymin box2
          (<= (ymin box1) (ymax box2))) ;; ymin box1 <= ymax box2
     (* (- (xmax box1) (xmin box2)) ;; (xmax box1 - xmin box2) * (ymax box2 - ymin box1)
        (- (ymax box2) (ymin box1)))]
    [else 0]))

;; Test:
;; (define box1 (cons 1 (cons 2 (cons 3 (cons 4 empty)))))
;; (define box2 (cons 3 (cons 4 (cons 5 (cons 6 empty)))))
;; (define box3 (cons 2 (cons 5 (cons 4 (cons 8 empty)))))

(define box4 (cons 3 (cons 5 (cons 3 (cons 8 empty)))))
(define box5 (cons 2 (cons 6 (cons 3 (cons 5 empty)))))
(define box6 (cons 1 (cons 2 (cons 1 (cons 2 empty)))))
(define box7 (cons 1.5 (cons 2.5 (cons 0.5 (cons 1.5 empty)))))
(define box8 (cons 1.5 (cons 2.5 (cons 1 (cons 2 empty)))))

(check-expect (overlap-area box2 box4) 6)
(check-expect (overlap-area box3 box5) 4)
(check-expect (overlap-area box6 box7) 0.25)
(check-expect (overlap-area box7 box8) 0.5)