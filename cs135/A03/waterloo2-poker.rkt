;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname waterloo2-poker) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 03
;*******************************************
;;

;;
;; Q2

;; Q2a
;; ********** a ************
;; (ordinality c) consumes one parameter: c
;; The function produces the ordinality of a Card c
;; ordinality: Card -> Int
;; (check-expect (ordinality Card) Int)

;; Examples:
(define card-c1 (cons 2 (cons 'S empty)))
(define card-c2 (cons 3 (cons 'H empty)))
(define card-c3 (cons 4 (cons 'D empty)))
(check-expect (ordinality card-c1) 2)
(check-expect (ordinality card-c2) 3)
(check-expect (ordinality card-c3) 4)

;; A Rank is one of: 2, 3, 4, 5, 6, 7, 8, 9, 10, 'J, 'Q, 'K, 'A
;; A Suit is one of: 'C, 'D, 'H, 'S
;; A Card is a: (cons Rank (cons Suit empty))

(define (ordinality c)
  (cond
   [(symbol? (first c))
    (cond
      [(symbol=? 'J (first c)) 11]
      [(symbol=? 'Q (first c)) 12]
      [(symbol=? 'K (first c)) 13]
      [(symbol=? 'A (first c)) 14])]
   [else (first c)]))

;; Tests:
(define card-c4 (cons 'A (cons 'C empty)))
(define card-c5 (cons 'Q (cons 'D empty)))
(check-expect (ordinality card-c4) 14)
(check-expect (ordinality card-c5) 12)

;;
;; Q2b 
;; ********** b ************
;;

;; suit: Card -> Suit
(define (suit card) (first (rest card)))

;; hand -> first-card-suit
(define (first-card-suit
         hand)
  (suit (first hand)))

;; hand -> second-card-suit
(define (second-card-suit
         hand)
  (suit (first (rest hand))))

;; hand -> first-card-ordinality
(define (first-card-ordinality
         hand)
  (ordinality (first hand)))

;; hand -> second-card-ordinality
(define (second-card-ordinality
         hand)
  (ordinality (first (rest hand))))

;;
;; (strength h) consumes one parameter: h
;; The function produces the strength of a Hand h.
;; strength: Hand -> Int
;; (check-expect (strength Hand) Int)

;; Examples:
(define hand1 (cons (cons 4 (cons 'S empty)) (cons (cons 4 (cons 'D empty)) empty)))
(check-expect (strength hand1) 3)

(define (strength
         h)
  (cond
    [(and (or (= (+ 1 (first-card-ordinality
                       h)) ;; the ordinality of the Cards have a difference of one.
                 (second-card-ordinality
                       h)) ;; card1 + 1 = card2
        (= (+ 1 (second-card-ordinality
                       h))
           (first-card-ordinality
                       h))) ;; or card2 + 1 = card1
         (symbol=? (first-card-suit h) ;; Also both cards have the same Suit.
                   (second-card-suit h))) 4] ;; straight flush (4)
    [(= (first-card-ordinality h)
        (second-card-ordinality h)) 3] ;; Cards have the same Rank -> pair (3).
    [(or (= (+ 1 (first-card-ordinality
                       h)) ;; the ordinality of the Cards have a difference of one.
                 (second-card-ordinality
                       h)) ;; card1 + 1 = card2
        (= (+ 1 (second-card-ordinality
                       h)) ;; or card2 + 1 = card1
           (first-card-ordinality
                       h))) 2] ;; ordinality of the Cards have a difference of one -> straight(2).
    [(symbol=? (first-card-suit h)
               (second-card-suit h)) 1] ;; both Cards have the same Suit -> flush (1).
    [else 0])) ;; non of the above (0)

;; Tests:

(define hand2 (cons (cons 3 (cons 'D empty)) (cons (cons 4 (cons 'D empty)) empty)))
(check-expect (strength hand2) 4)

(define hand3 (cons (cons 4 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))
(check-expect (strength hand3) 2)

(define hand4 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'S empty)) empty)))
(check-expect (strength hand4) 1)

(define hand5 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))
(check-expect (strength hand5) 0)

;;
;; Q2c
;; ********** c ************
;;

;; define the hightest ordinality in a hand
(define (biggest-ordinality
         hand)
  (cond
    [(> (first-card-ordinality hand)
      (second-card-ordinality hand)) (first-card-ordinality hand)]
    [else (second-card-ordinality hand)]))

;; define the lowest ordinality in a hand
(define (smallest-ordinality
         hand)
  (cond
    [(< (first-card-ordinality hand)
      (second-card-ordinality hand)) (first-card-ordinality hand)]
    [else (second-card-ordinality hand)]))

;; (hand<? h1 h2) consumes two parameter: h1 h2
;; The function produces two Hands h1 and h2
;; and produces true if h2 is a better Hand than h1, and false otherwise.
;; hand<?: hand hand -> bool
;; (check-expect (hand<? hand hand) bool)

;; Examples:
;;(define hand1 (cons (cons 4 (cons 'S empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand2 (cons (cons 3 (cons 'D empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand3 (cons (cons 4 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))
;;(define hand4 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'S empty)) empty)))
;;(define hand5 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))

(check-expect (hand<? hand1 hand2) true)
(check-expect (hand<? hand3 hand4) false)

(define (hand<?
         h1
         h2)
  (cond
    [(> (strength h2) (strength h1)) true] ;; strength of h2 is bigger
    [(and (= (strength h2) (strength h1)) ;; if the strengths of h1 and h2 are equal
          (> (biggest-ordinality h2)
             (biggest-ordinality h1))) true] ;; h2 has larger high ordinality
    [(and (= (strength h2) (strength h1)) ;; if the strengths of h1 and h2 are equal
          (= (biggest-ordinality h2)
             (biggest-ordinality h1)) ;; if the high ordinalities of h1 and h2 are equal
          (= (smallest-ordinality h2)
             (smallest-ordinality h1))) true] ;; h2 has larger low ordinality
    [else false])) ;; false otherwise

;; test:
(check-expect (hand<? hand4 hand5) false)
(check-expect (hand<? hand5 hand1) true)


;;
;; Q2d
;; ********** d ************
;;

;; (winner h1 h2) consumes two parameter: h1 h2
;; The function produces two Hands h1 and h2
;; and produces 'hand1 if h1 is a better Hand than h2
;; or 'hand2 if h2 is a better Hand than h1,
;; and 'tie if both Hands are equivalent.
;; winner: hand hand -> Sym
;; (check-expect (hand<? hand hand) Sym)

;; Examples
;;(define hand1 (cons (cons 4 (cons 'S empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand2 (cons (cons 3 (cons 'D empty)) (cons (cons 4 (cons 'D empty)) empty)))
;;(define hand3 (cons (cons 4 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))
;;(define hand4 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'S empty)) empty)))
;;(define hand5 (cons (cons 7 (cons 'S empty)) (cons (cons 5 (cons 'D empty)) empty)))

(check-expect (winner hand1 hand2) 'hand2)
(check-expect (winner hand3 hand4) 'hand1)

(define (winner
         h1
         h2)
  (cond
    [(hand<? h1 h2) 'hand2] ;; h2 is a better hand than h1
    [(and (= (strength h1) (strength h1)) ;; the strengths of h1 and h2 are same
          (= (biggest-ordinality h1) (biggest-ordinality h2)) ;; same high ordinality
          (= (smallest-ordinality h1) (smallest-ordinality h2))) ;;same low ordinality.
     'tie] 
    [else 'hand1])) ;; remaining case

;; Test:
(check-expect (winner hand4 hand5) 'tie)
(check-expect (winner hand5 hand1) 'hand2)







