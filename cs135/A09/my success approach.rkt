;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname |my success approach|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(define (transpose image)
  (reverse (foldr
            (lambda (row new-image)
              (cond
                [(empty? new-image) (map list row)]
                [else (map cons row new-image)]))
            empty
            (map reverse image))))

(transpose '((1 2 3 4) (5 6 7 8) (9 10 11 12) (13 14 15 16)))

(map
     cons
     (list 12 11 10 9)
     (list
      (list 16)
      (list 15)
      (list 14)
      (list 13)))

(define (transpose2 image)
  (map reverse
       (cond
         [(empty? (rest image)) (map (lambda (x) (list x)) (first image))]
         [else
          (foldl
           (lambda (x y) (map cons x y))
           (map (lambda (x) (list x)) (first image))
           (rest image))])))

(transpose2 '((1 2 3 4) (5 6 7 8) (9 10 11 12) (13 14 15 16)))