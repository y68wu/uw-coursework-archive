;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname cs135search) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 05
;;*******************************************
;;

;; (a)

;; (exist e list) produces the Str e if e is an element in the list
;; produces empty otherwise
;; exist: Str (listof Str) -> (anyof empty Str)

(define (exist e list)
  (cond
    [(empty? list) empty]
    [(string=? e (first list)) e]
    [else (exist e (rest list))]))

;; Tests:
(check-expect (exist "a" (list "e" "c" "d" "a")) "a")
(check-expect (exist "f" (list "e" "c" "d" "a")) empty)

;; (both DL1 DL2) consumes two DLs and produces a doc-list (DL) that occur in both DLs.
;; and produces empty otherwise
;; both: DL DL -> (listof DL)

;; Examples:

(check-expect (both (list "a.txt" "b.txt" "c.txt") (list "b.txt" "c.txt")) (list "b.txt" "c.txt"))
(check-expect (both (list "a.txt" "b.txt" "c.txt" "d.txt" "e.txt" "f.txt")
                    (list "b.txt" "c.txt" "o.txt" "v.txt" "w.txt")) (list "b.txt" "c.txt"))

(define (both DL1 DL2)
  (cond
    [(empty? DL1) empty]
    [(empty? (exist (first DL1) DL2))
     (both (rest DL1) DL2)]
    [else (cons (first DL1) (both (rest DL1) DL2))]))

;; Tests:

(check-expect (both (list "b.txt") (list "b.txt" "c.txt")) (list "b.txt"))
(check-expect (both (list "d.txt") (list "b.txt" "c.txt")) empty)

;; (b)

;; (exclude DL1 DL2) consumes two DLs and produces a doc-list (DL)
;; that occur in the first DL but not the second one.
;; The function provide empty otherwise
;; exclude: DL DL -> (listof DL)

;; Examples:

(check-expect (exclude (list "b.txt" "c.txt") (list "b.txt")) (list "c.txt"))
(check-expect (exclude (list "a.txt" "b.txt" "c.txt") (list "b.txt" "c.txt")) (list "a.txt"))

(define (exclude DL1 DL2)
  (cond
    [(empty? DL1) empty]
    [(empty? (exist (first DL1) DL2))
     (cons (first DL1) (exclude (rest DL1) DL2))]
    [else (exclude (rest DL1) DL2)]))

;; Tests:

(check-expect (exclude (list "b.txt" "c.txt") (list "b.txt" "c.txt")) empty)
(check-expect (exclude (list "b.txt" "c.txt" "d.txt") (list "c.txt")) (list "b.txt" "d.txt"))

;; (c)

;; sort a listof Str with lexicographic ordering

(define (order list)
  (cond
    [(empty? list) empty]
    [else (insert (first list) (order (rest list)))]))

(define (insert n list)
  (cond
    [(empty? list) (cons n empty)]
    [(string<=? n (first list)) (cons n list)]
    [else (cons (first list) (insert n (rest list)))]))

;; Examples:

(check-expect (order (list "b" "d" "a")) (list "a" "b" "d"))
(check-expect (order (list "b" "d" "a" "e" "f")) (list "a" "b" "d" "e" "f"))

;; ;; (retrieve doc an-il) consumes a Str and an IL
;; The function produces a (listof Str) without lexicographic ordering.

(define (retrieve doc an-il)
  (cond
    [(empty? an-il) empty]
    [(empty? (exist doc (first (rest (first an-il)))))
     (retrieve doc (rest an-il))]
    [else (cons (first (first an-il))
                (retrieve doc (rest an-il)))]))

;; Examples:

(check-expect (retrieve "b.txt" InvertedList) (list "barks" "dog" "the"))
(check-expect (retrieve "c.txt" InvertedList) (list "cat" "chases" "dog" "suddenly" "the"))

;; (keys-retrieve doc an-il) consumes a Str and an IL
;; The function produces a (listof Str) with lexicographic ordering.
;; The values in the produced list are the keys from an-il whose doc-lists contain doc.
;; If doc is not contained in the doc-list associated with any keys in an-il,
;; then keys-retrieve returns empty.
;; keys-retrieve: Str IL -> (listof Str)

;; Examples:

(define InvertedList (list (list "barks" (list "b.txt"))
                           (list "cat" (list "a.txt" "c.txt"))
                           (list "chases" (list "c.txt"))
                           (list "dog" (list "b.txt" "c.txt"))
                           (list "sleeps" (list "a.txt"))
                           (list "suddenly" (list "c.txt"))
                           (list "the" (list "a.txt" "b.txt" "c.txt"))))

(check-expect (keys-retrieve "a.txt" InvertedList) (list "cat" "sleeps" "the"))

(define (keys-retrieve doc an-il)
  (order (retrieve doc an-il)))

;; Tests

(check-expect (keys-retrieve "b.txt" InvertedList) (list "barks" "dog" "the"))
(check-expect (keys-retrieve "c.txt" InvertedList) (list "cat" "chases" "dog" "suddenly" "the"))

;; (d)

;; find DL that the key is str in an IL
;; if the key is not in IL, the function produces empty

(define (find str an-il)
  (cond
    [(empty? an-il) empty]
    [(string=? str (first (first an-il))) (first (rest (first an-il)))]
    [else (find str (rest an-il))]))

;; Examples:
(check-expect (find "cat" InvertedList) (list "a.txt" "c.txt"))
(check-expect (find "suddenly" InvertedList) (list "c.txt"))
(check-expect (find "s" InvertedList) empty)

;; Case 1:

;; (bothkey str1 str2 an-il) given two keys str1 and str2 from an-il,
;; produces a doc-list (DL) containing the documents that are present in both of the keys’
;; associated doc-lists. The function produces empty otherwises.

(define (bothkey str1 str2 an-il)
  (cond
    [(empty? (find str1 an-il)) empty]
    [(empty? (find str2 an-il)) empty]
    [else (both (find str1 an-il) (find str2 an-il))]))

;; Examples:
(check-expect (bothkey "barks" "dog" InvertedList) (list "b.txt"))
(check-expect (bothkey "barks" "chases" InvertedList) empty)
(check-expect (bothkey "dog" "the" InvertedList) (list "b.txt" "c.txt"))

;; Case 2:

;; (excludekey str1 str2 an-il) given two keys str1 and str2 from an-il, produces a doc-list (DL)
;; containing the documents that are present in the doc-list associated with the key str1,
;; but not the key str2. The function produces empty otherwises.

(define (excludekey str1 str2 an-il)
  (cond
    [(empty? (find str1 an-il)) empty]
    [(empty? (find str2 an-il)) empty]
    [else (exclude (find str1 an-il) (find str2 an-il))]))

;; Examples:
(check-expect (excludekey "barks" "chases" InvertedList) (list "b.txt"))
(check-expect (excludekey "cat" "sleeps" InvertedList) (list "c.txt"))
(check-expect (excludekey "cat" "the" InvertedList) empty)

;; (search sym str1 str2 an-il) consumes a Sym, two Strs and an IL.
;; The function produces a doc-list (DL). The arguments for search will always be in one of
;; two possible cases above:
;; (search 'both str1 str2 an-il) and 
;; (search 'exclude str1 str2 an-il) respectively
;; search: Sym Str Str IL -> (listof DL)

;; Examples
(check-expect (search 'both "barks" "dog" InvertedList) (list "b.txt"))
(check-expect (search 'exclude "barks" "chases" InvertedList) (list "b.txt"))

(define (search sym str1 str2 an-il)
  (cond
    [(symbol=? sym 'both) (bothkey str1 str2 an-il)]
    [(symbol=? sym 'exclude) (excludekey str1 str2 an-il)]
    [else empty]))

;; Tests
(check-expect (search 'both "dog" "the" InvertedList) (list "b.txt" "c.txt"))
(check-expect (search 'exclude "cat" "sleeps" InvertedList) (list "c.txt"))