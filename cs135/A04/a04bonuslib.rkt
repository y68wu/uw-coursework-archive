
;; Ignore the next three lines
#lang racket
(require test-engine/racket-tests)
(provide last-digit other-digits)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; BEGIN SUPPORT FUNCTIONS: DO NOT MODIFY ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; ------------ Bonus ----------------

;; A Digit is one of 0, 1, 2, 3, 4, 5, 6, 7, 8, 9.
;; A DecInt is one of:
;; * A Digit
;; * (+ (* b 10) d) where b is a DecInt and d is a Digit

;; (last-digit n) returns the last digit (base 10) of n
;; Example:
(check-expect (last-digit 123) 3)

;; last-digit:  DecInt -> Digit
(define (last-digit n)
  (remainder n 10))

;; Tests:
(check-expect (last-digit 0) 0)
(check-expect (last-digit 1) 1)

;; (other-digits n) removes the last digit of n and returns the other digits
;; Example:
(check-expect (other-digits 12345) 1234)

;; other-digits: DecInt -> DecInt 
(define (other-digits n)
  (quotient n 10))

;; Test:
(check-expect (other-digits 0) 0)


;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; END SUPPORT FUNCTIONS ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;

