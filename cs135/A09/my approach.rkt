;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname |my approach|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(define (transpose lst)
  (build-list (length (first lst))
              (lambda (x)
                (foldr (lambda (n acc)
                         (cons (list-ref n x) acc))
                       empty
                       lst))))

(check-expect (transpose '((1 0 0 1 1))) '((1) (0) (0) (1) (1)))

(check-expect (transpose '((1 2 3 4) (5 6 7 8) (9 10 11 12) (13 14 15 16)))
              '((1 5 9 13) (2 6 10 14) (3 7 11 15) (4 8 12 16)))

(define (transpose2 lst)
  (build-list (length (first lst))
              (lambda (x)
                (reverse (foldl
                          (lambda (n acc)
                            (cons (list-ref n x) acc))
                          empty
                          lst)))))

(transpose2 '((1 2 3 4) (5 6 7 8) (9 10 11 12) (13 14 15 16)))