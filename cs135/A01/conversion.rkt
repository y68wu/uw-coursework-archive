;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname conversion) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 01
;*******************************************
;;

;; (a)
(define miles->meters 1609.344)
(define hours->seconds (* 60 60))

(define (mph->m/s speed-in-mph)
  (* speed-in-mph (/ miles->meters hours->seconds)))

;; (b)
(define lbf->Newtons 4.4482)
(define inch->meters (/ 0.3048 12))

(define (psi->pa pressure-in-psi)
  (* pressure-in-psi (/ lbf->Newtons (sqr inch->meters))))


;; (c)
(define foot->meters 0.3048)

(define (lbf-ft->Nm torque-in-lbf-ft)
  (* torque-in-lbf-ft lbf->Newtons foot->meters))

;; (d)
(define fornights->days 14)
(define day-hours 24)
(define fathom->meters 1.8288)
(define fornights->hours (* fornights->days day-hours))

(define (fpf->mph speed-in-fpf)
  (* speed-in-fpf (/ (/ fathom->meters miles->meters) fornights->hours)))


