;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname blood) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 02
;*******************************************
;;

;;
; (a)
; (can-donate-to/cond? donor-blood-type recipient-blood-type)
; The function above takes two parameters: a symbol representing the blood type of the donor
; and a symbol representing the blood type of the recipient.
; The function produces true if the donor’s blood type is acceptable for the recipient’s blood type
; (according to the chart), and false otherwise.
; can-donate-to/cond? Sym Sym -> Bool
; (check-expect (can-donate-to/cond? Sym Sym) Bool)
;; Examples:

(check-expect (can-donate-to/cond? 'O- 'O-) true)
(check-expect (can-donate-to/cond? 'O- 'O+) true)
(check-expect (can-donate-to/cond? 'O- 'A-) true)
(check-expect (can-donate-to/cond? 'O- 'A+) true)
(check-expect (can-donate-to/cond? 'O- 'B-) true)
(check-expect (can-donate-to/cond? 'O- 'B+) true)
(check-expect (can-donate-to/cond? 'O- 'AB-) true)
(check-expect (can-donate-to/cond? 'O- 'AB+) true)

(check-expect (can-donate-to/cond? 'O+ 'O-) false)
(check-expect (can-donate-to/cond? 'O+ 'O+) true)
(check-expect (can-donate-to/cond? 'O+ 'A-) false)
(check-expect (can-donate-to/cond? 'O+ 'A+) true)
(check-expect (can-donate-to/cond? 'O+ 'B-) false)
(check-expect (can-donate-to/cond? 'O+ 'B+) true)
(check-expect (can-donate-to/cond? 'O+ 'AB-) false)
(check-expect (can-donate-to/cond? 'O+ 'AB+) true)

(check-expect (can-donate-to/cond? 'A- 'O-) false)
(check-expect (can-donate-to/cond? 'A- 'O+) false)
(check-expect (can-donate-to/cond? 'A- 'A-) true)
(check-expect (can-donate-to/cond? 'A- 'A+) true)
(check-expect (can-donate-to/cond? 'A- 'B-) false)
(check-expect (can-donate-to/cond? 'A- 'B+) false)
(check-expect (can-donate-to/cond? 'A- 'AB-) true)
(check-expect (can-donate-to/cond? 'A- 'AB+) true)

(check-expect (can-donate-to/cond? 'A+ 'O-) false)
(check-expect (can-donate-to/cond? 'A+ 'O+) false)
(check-expect (can-donate-to/cond? 'A+ 'A-) false)
(check-expect (can-donate-to/cond? 'A+ 'A+) true)
(check-expect (can-donate-to/cond? 'A+ 'B-) false)
(check-expect (can-donate-to/cond? 'A+ 'B+) false)
(check-expect (can-donate-to/cond? 'A+ 'AB-) false)
(check-expect (can-donate-to/cond? 'A+ 'AB+) true)

(check-expect (can-donate-to/cond? 'B- 'O-) false)
(check-expect (can-donate-to/cond? 'B- 'O+) false)
(check-expect (can-donate-to/cond? 'B- 'A-) false)
(check-expect (can-donate-to/cond? 'B- 'A+) false)
(check-expect (can-donate-to/cond? 'B- 'B-) true)
(check-expect (can-donate-to/cond? 'B- 'B+) true)
(check-expect (can-donate-to/cond? 'B- 'AB-) true)
(check-expect (can-donate-to/cond? 'B- 'AB+) true)

(check-expect (can-donate-to/cond? 'B+ 'O-) false)
(check-expect (can-donate-to/cond? 'B+ 'O+) false)
(check-expect (can-donate-to/cond? 'B+ 'A-) false)
(check-expect (can-donate-to/cond? 'B+ 'A+) false)
(check-expect (can-donate-to/cond? 'B+ 'B-) false)
(check-expect (can-donate-to/cond? 'B+ 'B+) true)
(check-expect (can-donate-to/cond? 'B+ 'AB-) false)
(check-expect (can-donate-to/cond? 'B+ 'AB+) true)

(check-expect (can-donate-to/cond? 'AB- 'O-) false)
(check-expect (can-donate-to/cond? 'AB- 'O+) false)
(check-expect (can-donate-to/cond? 'AB- 'A-) false)
(check-expect (can-donate-to/cond? 'AB- 'A+) false)
(check-expect (can-donate-to/cond? 'AB- 'B-) false)
(check-expect (can-donate-to/cond? 'AB- 'B+) false)
(check-expect (can-donate-to/cond? 'AB- 'AB-) true)
(check-expect (can-donate-to/cond? 'AB- 'AB+) true)

(check-expect (can-donate-to/cond? 'AB+ 'O-) false)
(check-expect (can-donate-to/cond? 'AB+ 'O+) false)
(check-expect (can-donate-to/cond? 'AB+ 'A-) false)
(check-expect (can-donate-to/cond? 'AB+ 'A+) false)
(check-expect (can-donate-to/cond? 'AB+ 'B-) false)
(check-expect (can-donate-to/cond? 'AB+ 'B+) false)
(check-expect (can-donate-to/cond? 'AB+ 'AB-) false)
(check-expect (can-donate-to/cond? 'AB+ 'AB+) true)

(define (can-donate-to/cond?
         donor-blood-type
         recipient-blood-type)
  (cond
    [(symbol=? recipient-blood-type 'AB+) true] ;; boold type of recipient is AB+
    [(symbol=? donor-blood-type 'O-) true] ;; boold type of donor is O-
    [(symbol=? donor-blood-type recipient-blood-type) true] ;; same boold type for recipient and donor
    [(symbol=? donor-blood-type 'O+) ;; boold type of donor is O+
     (cond
       [(symbol=? recipient-blood-type 'A+) true] ;; A+ can accept O+
       [(symbol=? recipient-blood-type 'B+) true] ;; B+ can accept O+
       [else false])]
    [(symbol=? donor-blood-type 'A-) ;; boold type of donor is A-
     (cond
       [(symbol=? recipient-blood-type 'A+) true] ;; A+ can accept A-
       [(symbol=? recipient-blood-type 'AB-) true] ;; AB- can accept A-
       [else false])]
    [(symbol=? donor-blood-type 'B-) ;; boold type of donor is B-
     (cond
       [(symbol=? recipient-blood-type 'B+) true] ;; B+ can accept B-
       [(symbol=? recipient-blood-type 'AB-) true] ;; AB- can accept B-
       [else false])]
    [else false]
    ))

;;Tested:
(check-expect (can-donate-to/cond? 'O- 'O-) true)
(check-expect (can-donate-to/cond? 'O+ 'O-) false)
(check-expect (can-donate-to/cond? 'A- 'O-) false)
(check-expect (can-donate-to/cond? 'B- 'O-) false)
(check-expect (can-donate-to/cond? 'B- 'O-) false)
(check-expect (can-donate-to/cond? 'B+ 'O-) false)
(check-expect (can-donate-to/cond? 'AB- 'O-) false)
(check-expect (can-donate-to/cond? 'AB+ 'O-) false)

;; (b)
; (can-donate-to/bool? donor-blood-type recipient-blood-type)
; The function above takes two parameters: a symbol representing the blood type of the donor
; and a symbol representing the blood type of the recipient.
; The function produces true if the donor’s blood type is acceptable for the recipient’s blood type
; (according to the chart), and false otherwise.
; can-donate-to/bool? Sym Sym -> Bool
; (check-expect (can-donate-to/bool? Sym Sym) Bool)
;; Examples:

(check-expect (can-donate-to/bool? 'O- 'O-) true)
(check-expect (can-donate-to/bool? 'O- 'O+) true)
(check-expect (can-donate-to/bool? 'O- 'A-) true)
(check-expect (can-donate-to/bool? 'O- 'A+) true)
(check-expect (can-donate-to/bool? 'O- 'B-) true)
(check-expect (can-donate-to/bool? 'O- 'B+) true)
(check-expect (can-donate-to/bool? 'O- 'AB-) true)
(check-expect (can-donate-to/bool? 'O- 'AB+) true)

(check-expect (can-donate-to/bool? 'O+ 'O-) false)
(check-expect (can-donate-to/bool? 'O+ 'O+) true)
(check-expect (can-donate-to/bool? 'O+ 'A-) false)
(check-expect (can-donate-to/bool? 'O+ 'A+) true)
(check-expect (can-donate-to/bool? 'O+ 'B-) false)
(check-expect (can-donate-to/bool? 'O+ 'B+) true)
(check-expect (can-donate-to/bool? 'O+ 'AB-) false)
(check-expect (can-donate-to/bool? 'O+ 'AB+) true)

(check-expect (can-donate-to/bool? 'A- 'O-) false)
(check-expect (can-donate-to/bool? 'A- 'O+) false)
(check-expect (can-donate-to/bool? 'A- 'A-) true)
(check-expect (can-donate-to/bool? 'A- 'A+) true)
(check-expect (can-donate-to/bool? 'A- 'B-) false)
(check-expect (can-donate-to/bool? 'A- 'B+) false)
(check-expect (can-donate-to/bool? 'A- 'AB-) true)
(check-expect (can-donate-to/bool? 'A- 'AB+) true)

(check-expect (can-donate-to/bool? 'A+ 'O-) false)
(check-expect (can-donate-to/bool? 'A+ 'O+) false)
(check-expect (can-donate-to/bool? 'A+ 'A-) false)
(check-expect (can-donate-to/bool? 'A+ 'A+) true)
(check-expect (can-donate-to/bool? 'A+ 'B-) false)
(check-expect (can-donate-to/bool? 'A+ 'B+) false)
(check-expect (can-donate-to/bool? 'A+ 'AB-) false)
(check-expect (can-donate-to/bool? 'A+ 'AB+) true)

(check-expect (can-donate-to/bool? 'B- 'O-) false)
(check-expect (can-donate-to/bool? 'B- 'O+) false)
(check-expect (can-donate-to/bool? 'B- 'A-) false)
(check-expect (can-donate-to/bool? 'B- 'A+) false)
(check-expect (can-donate-to/bool? 'B- 'B-) true)
(check-expect (can-donate-to/bool? 'B- 'B+) true)
(check-expect (can-donate-to/bool? 'B- 'AB-) true)
(check-expect (can-donate-to/bool? 'B- 'AB+) true)

(check-expect (can-donate-to/bool? 'B+ 'O-) false)
(check-expect (can-donate-to/bool? 'B+ 'O+) false)
(check-expect (can-donate-to/bool? 'B+ 'A-) false)
(check-expect (can-donate-to/bool? 'B+ 'A+) false)
(check-expect (can-donate-to/bool? 'B+ 'B-) false)
(check-expect (can-donate-to/bool? 'B+ 'B+) true)
(check-expect (can-donate-to/bool? 'B+ 'AB-) false)
(check-expect (can-donate-to/bool? 'B+ 'AB+) true)

(check-expect (can-donate-to/bool? 'AB- 'O-) false)
(check-expect (can-donate-to/bool? 'AB- 'O+) false)
(check-expect (can-donate-to/bool? 'AB- 'A-) false)
(check-expect (can-donate-to/bool? 'AB- 'A+) false)
(check-expect (can-donate-to/bool? 'AB- 'B-) false)
(check-expect (can-donate-to/bool? 'AB- 'B+) false)
(check-expect (can-donate-to/bool? 'AB- 'AB-) true)
(check-expect (can-donate-to/bool? 'AB- 'AB+) true)

(check-expect (can-donate-to/bool? 'B- 'O-) false)
(check-expect (can-donate-to/bool? 'B- 'O+) false)
(check-expect (can-donate-to/bool? 'B- 'A-) false)
(check-expect (can-donate-to/bool? 'B- 'A+) false)
(check-expect (can-donate-to/bool? 'B- 'B-) true)
(check-expect (can-donate-to/bool? 'B- 'B+) true)
(check-expect (can-donate-to/bool? 'B- 'AB-) true)
(check-expect (can-donate-to/bool? 'B- 'AB+) true)

(define (can-donate-to/bool?
         donor-blood-type
         recipient-blood-type)
  (or (symbol=? donor-blood-type recipient-blood-type) ;; same boold type for recipient and donor
      (symbol=? donor-blood-type 'O-) ;; boold type of donor is O-
      (symbol=? recipient-blood-type 'AB+) ;; boold type of recipient is AB+
      (and (symbol=? donor-blood-type 'O+) ;; boold type of donor is O+
           (or
            (symbol=? recipient-blood-type 'A+) ;; A+ can accept O+
            (symbol=? recipient-blood-type 'B+))) ;; B+ can accept O+
      (and (symbol=? donor-blood-type 'A-) ;; boold type of donor is A-
           (or
            (symbol=? recipient-blood-type 'A+) ;; A+ can accept A-
            (symbol=? recipient-blood-type 'AB-))) ;; AB- can accept A-
      (and (symbol=? donor-blood-type 'B-) ;; boold type of donor is B-
           (or
            (symbol=? recipient-blood-type 'B+) ;; B+ can accept B-
            (symbol=? recipient-blood-type 'AB-))))) ;; AB- can accept B-

;;Tested:
(check-expect (can-donate-to/bool? 'O- 'O-) true)
(check-expect (can-donate-to/bool? 'O+ 'O-) false)
(check-expect (can-donate-to/bool? 'A- 'O-) false)
(check-expect (can-donate-to/bool? 'B- 'O-) false)
(check-expect (can-donate-to/bool? 'B- 'O-) false)
(check-expect (can-donate-to/bool? 'B+ 'O-) false)
(check-expect (can-donate-to/bool? 'AB- 'O-) false)
(check-expect (can-donate-to/bool? 'AB+ 'O-) false)






