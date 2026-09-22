;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname hot-dog) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 04
;;*******************************************
;;

;; ********Q3a********

;; (contains-hot-dog? list) consumes a list of Any 
;; The function produces true if the list contains the symbol 'hot-dog and false otherwise.
;; The symbol must be exactly 'hot-dog (all in lower/down case with a hyphen in the middle).
;; contains-hot-dog?: listof Any -> Bool
;; (check-expect (contains-hot-dog? (listof Any)) Bool)

;; Example:
(define l1 (cons 'pizza (cons 'hot-dog (cons 'hamburger empty))))
(check-expect (contains-hot-dog? l1) true)

(define (contains-hot-dog? list)
  (cond
    [(empty? list) false] ;; if the list is empty
    [(symbol? (first list)) ;; if the first element of the list is symbol
     (cond
       [(symbol=? 'hot-dog (first list)) true] ;; if the the first element is 'hot-dog
       [else (contains-hot-dog? (rest list))])] ;; recursion
    [else (contains-hot-dog? (rest list))])) ;; recursion

;; Test:
(define l2 (cons "pizza" (cons "hot-dog" (cons 2 empty))))
(define l3 (cons 6 (cons 'hot-dog (cons "Good" empty))))
(check-expect (contains-hot-dog? l2) false)
(check-expect (contains-hot-dog? l3) true)

;; ********Q3b********

;; (string->list "hot dog")
;; (cons #\h (cons #\o (cons #\t (cons #\space (cons #\d (cons #\o (cons #\g empty)))))))

;; (find-e str e) consumes two parameters: str, e 
;; The function produces true if we can find the letter of Char e (accept capital) in str 
;; and false otherwise 
;; find-e: Str Char -> Bool
;; (check-expect (find-e Str Char) Bool)

;; Example:
(check-expect (find-e "Spring" #\n) true)

(define (find-e str e)
  (cond
    [(empty? (string->list (string-downcase str))) false]
    [(char=? (first (string->list (string-downcase str))) e) true]
    [else (find-e (list->string (rest (string->list (string-downcase str)))) e)]))

;; Tests:
(check-expect (find-e "Spring" #\s) true)
(check-expect (find-e "Spring" #\N) false)

;; (times str a) consumes two parameters: str, a 
;; The function produces the number of times that the letter of Char e (accept capital)
;; appear in str 
;; times: Str Char -> Int
;; (check-expect (times Str Char) Int)

;; Examples:
(check-expect (times "spring" #\s) 1)
(check-expect (times "Sspring" #\s) 2)

(define (times str a)
  (cond
    [(empty? (string->list (string-downcase str))) 0]
    [(char=? (first (string->list (string-downcase str))) a)
     (+ 1 (times (list->string (rest (string->list (string-downcase str)))) a))]
    [else (times (list->string (rest (string->list (string-downcase str)))) a)]))

;; Tests:
(check-expect (times "hot dog" #\o) 2)
(check-expect (times "hot dOg" #\o) 2)
(check-expect (times "hOt dOg" #\o) 2)

;; (spells-hot-dog? str) consumes a String
;; The function produces true if it contains the letters required to spell "hot dog"
;; and false otherwise.
;; In other words, it contains at least one of each of the following:
;; {h, t, d, g, space} and at least two o’s.
;; The letters may appear in the string as either upper or lower/down case letters.
;; spells-hot-dog?: Str -> Bool
;; (check-expect (spells-hot-dog? Str) Bool)

;; Examples:
(check-expect (spells-hot-dog? "hotdog") false)
(check-expect (spells-hot-dog? "hot dog") true)

(define (spells-hot-dog? str)
  (cond
    [(and (find-e str #\h) ;; contain h
          (find-e str #\o) ;; contain o
          (find-e str #\t) ;; contain t
          (find-e str #\space) ;; contain space
          (find-e str #\d) ;; contain d
          (find-e str #\g) ;; contain g
          (>= (times str #\o) 2)) true] ;; contain o at least twice
    [else false]))

;; Tests:
(check-expect (spells-hot-dog? "Hot dOG") true)
(check-expect (spells-hot-dog? "hot dg") false)