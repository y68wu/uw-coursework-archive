;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname box-office) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 02
;*******************************************
;;

;; define moive name bonus/penalty
(define movie-name-bonus 25)
(define movie-name-penalty -50)

;; define moive name profits
(define (movie-name-profits
         movie-name)
  (cond
    [(< (string-length movie-name) 10) movie-name-bonus]
    [else 0]))

;; define moive name penalty
(define (moive-name-penalty
         movie-name)
  (cond
    [(string=? (substring movie-name 0 3) "The") movie-name-penalty]
    [else 0]))

;; define studio name bonus/penalty
(define studio-name-bonus 500)
(define studio-name-penalty -250)

;; define studio name profits
(define (studio-name-profits
         studio-name)
  (cond
    [(string=? studio-name "Marvel") studio-name-bonus]
    [(string=? studio-name "DC") studio-name-penalty]
    [else 0]))

;; define famous actors bonus
(define famous-actors-bonus 50)

;; define famous actors name profits
(define (famous-actors-profits
         famous-actors-num)
  (* famous-actors-num famous-actors-bonus))

;; define explosions bonus/penalty
(define explosions-bonus 6)
(define explosions-penalty -20)

;; define explosions profits
(define (explosions-profits
         explosions-num)
  (+ explosions-penalty
     (* explosions-num explosions-bonus)))


;;
; (box-office-profits movie-name studio-name famous-actors-num explosions-num)
; consumes four parameters: 1. The name of the movie (as a Str)
;                           2. The name of a studio (as a Str)
;                           3. The number of famous actors (as a Nat)
;                           4. The number of explosions (as a Nat)
; The function produces the total box office profits (as an Num) in millions.
; box-office-profit: Str Str Nat Nat -> Num
; (check-expect (box-office-profits Str Str Nat Nat) Num)
;; Examples:
(check-expect (box-office-profits "Thor: Love and Thunder" "Marvel" 4 50) 980)
(check-expect (box-office-profits "The" "DC" 6 30) 185)

(define (box-office-profits
         movie-name
         studio-name
         famous-actors-num
         explosions-num)
  (+ (movie-name-profits movie-name)
     (moive-name-penalty movie-name)
     (studio-name-profits studio-name)
     (famous-actors-profits famous-actors-num)
     (explosions-profits explosions-num)))

;; test
(check-expect (box-office-profits "Thor: Love and Thunder" "Marvel" 4 50) 980)