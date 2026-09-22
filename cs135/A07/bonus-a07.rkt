;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname bonus) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 07
;;*******************************************
;;


(require "fs-print.rkt")

;; *********** Bonus *************


;; fd-acc: Str FileDir (listof Str) (listof Str) -> (listof Str)

;; Examples:

(check-expect (fd-acc "vacation" (dir-contents sample-fs) empty empty) empty)
(check-expect (fd-acc "shopping.txt" (dir-contents sample-fs) empty empty) empty)

(define (fd-acc name fd record-path acc)
  (cond
    [(and (dir? fd) (string=? name (dir-name fd)))
     (fdlst-acc name (dir-contents fd) (cons name record-path)
                (cons (reverse (cons name record-path)) acc))]
    [(dir? fd) (fdlst-acc name (dir-contents fd) (cons (dir-name fd) record-path) acc)]
    [(and (file? fd) (string=? name (file-name fd))) (cons (reverse (cons name record-path)) acc)]
    [else acc]))

;; fdlst-acc: Str FDList (listof Str) (listof Str) -> (listof Str)

;; Examples:

(check-expect (fdlst-acc "vacation" (dir-contents sample-fs) empty empty)
              (list (list "photos" "vacation")))

(check-expect (fdlst-acc "shopping.txt" (dir-contents sample-fs) empty empty)
              (list (list "notes" "shopping.txt")))

(define (fdlst-acc name fdlst record-path acc)
  (cond
    [(empty? fdlst) acc]
    [else (fdlst-acc name (rest fdlst) record-path (fd-acc name (first fdlst) record-path acc))]))

;; (find name dir) comsumes two parameters name, dir and it finds every file and directory
;; contained in dir that has the given name.
;; The function produces a list of paths to those files and directories or empty
;; if there is no file or directory with the given name.
;; find: Str FileDir -> (listof (listof Str))

;; Examples:

(check-expect (find "vacation" sample-fs) (list (list "root" "photos" "vacation")))
(check-expect (find "shopping.txt" sample-fs) (list (list "root" "notes" "shopping.txt")))

(define (find name dir)
  (fd-acc name dir empty empty))

;; Tests

(check-expect (find "beach1.jpg" sample-fs)
              (list (list "root" "photos" "beach1.jpg")
                    (list "root" "photos" "vacation" "beach1.jpg")))
(check-expect (find "readme.txt" (make-file "readme.txt" 187 1319502441))
              (list (list "readme.txt")))