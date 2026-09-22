;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname alf) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 09
;;*******************************************
;;


;; ************* q2a **************

(define (absolutely-odd lst)
  (foldr + 0 (filter (lambda (x) (odd? x)) (map abs lst))))

(check-expect (absolutely-odd '(1 -5 4 6 5)) 11)
(check-expect (absolutely-odd '()) 0)

;; ************* q2b **************

(define (unzip lst)
  (list (map first lst)
        (map second lst)))

(check-expect (unzip '((1 a)(2 b)(3 c))) '((1 2 3) (a b c)))
(check-expect (unzip '()) '(()()))

;; ************* q2c ***************

(define (dedup lst)
  (foldr (lambda (fl rl) (cons fl (filter (lambda (x) (not (= x fl))) rl))) empty lst))

(check-expect (dedup '(1 2 1 3 3 2 4)) '(1 2 3 4))

;; ************* q2d ***************

(define (zero-fill str)
  (list->string
   (foldl cons
          (string->list str)
          (build-list (- 20 (length (string->list str))) (lambda (x) #\0)))))

(check-expect (zero-fill "abcdefghijklmn") "000000abcdefghijklmn")
(check-expect (zero-fill "he00llo") "0000000000000he00llo")

;; ************* q2e ***************

(define (subsequence lst from to)
  (reverse (first (foldl
                   (lambda (flst position)
                     (cond
                       [(and (>= (second position) from) (<= (second position) (sub1 to)))
                        (list (cons flst (first position)) (add1 (second position)))]
                       [else (list (first position) (add1 (second position)))]))
                   (list empty 0)
                   lst))))

(check-expect (subsequence '(2 4 5 6) 0 2) '(2 4))
(check-expect (subsequence '( 2 4 5 6) 0 1) '(2 ))


;; ************* q2f ***************

(define (occ lst num)
  (length (filter (lambda (x) (= x num)) lst)))

(check-expect (occ (list 1 1 1 2 1) 1) 4)
(check-expect (occ (list 1 2 3) 4) 0)

;; ************* q2g ***************

(define (pocket-change lst)
  (foldl + 0
         (map (lambda (x)
                (cond
                  [(and (symbol? x) (symbol=? x 'penny)) 0.01]
                  [(and (symbol? x) (symbol=? x 'nickel)) 0.05]
                  [(and (symbol? x) (symbol=? x 'dime)) 0.1]
                  [(and (symbol? x) (symbol=? x 'quarter)) 0.25]
                  [(and (symbol? x) (symbol=? x 'loonie)) 1.0]
                  [(and (symbol? x) (symbol=? x 'toonie)) 2.0]
                  [else 0])) lst)))

(check-expect (pocket-change (list 'dime 'dime 'penny 'fluff)) 0.21)
(check-expect (pocket-change (list 'loonie)) 1)

;; ************* q2h ***************

(define (sum-at-zero lst)
  (foldl + 0 (map (lambda (x) (x 0)) lst)))

(check-expect (sum-at-zero (list add1 sqr add1)) 2)