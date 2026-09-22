;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname tictactoe) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 05
;;*******************************************
;;

;; (a)

;; find the times that an element e occurs in a list
(define (lnum list e)
  (cond
    [(empty? list) 0]
    [(symbol=? (first list) e)
     (+ 1 (lnum (rest list) e))]
    [else (lnum (rest list) e)]))

;; Examples:
(check-expect (lnum (list 'X 's 'X) 'X) 2)
(check-expect (lnum (list '_ '_ 'O) 'O) 1)
(check-expect (lnum (list '_ '_ 'O) 'X) 0)

;; find the times that an element e occurs in a T3Grid
(define (Tnum T3Grid e)
  (cond
    [(empty? T3Grid) 0]
    [else (+ (lnum (first T3Grid) e)
             (Tnum (rest T3Grid) e))]))

;; Examples:
(define grid1
(list (list '_ '_ '_)
      (list '_ '_ '_)
      (list '_ '_ '_)))

(check-expect (Tnum grid1 'X) 0)
(check-expect (Tnum grid1 'O) 0)

;; (whose-turn T3Grid) that consumes a T3Grid and determines whose turn it is.
;; X goes first, so if the number of Xs and Os is equal, X goes next, the function produces 'X.
;; If the number of Xs is 1 greater than the number of Os, O goes next the function produces 'O.
;; whose-turb: T3Grid -> (anyof 'O 'X)

;; Examples:

(define grid2
(list (list 'X 'O 'O 'O '_)
      (list 'X 'X 'O '_ '_)
      (list '_ '_ '_ '_ '_)
      (list '_ '_ '_ '_ '_)
      (list '_ '_ '_ '_ 'X)))

(define grid3 (list (list 'X)))

(check-expect (whose-turn grid1) 'X)

(define (whose-turn T3Grid)
  (cond
    [(= (Tnum T3Grid 'X) (Tnum T3Grid 'O)) 'X]
    [(= (Tnum T3Grid 'X) (+ 1 (Tnum T3Grid 'O))) 'O]))

;; Tests:
(check-expect (whose-turn grid2) 'X)
(check-expect (whose-turn grid3) 'O)

;; (b)

;; produces the number of rows and columns in the grid

(define (NumR/C T3Grid)
  (cond
    [(empty? T3Grid) 0]
    [else (+ 1 (NumR/C (rest T3Grid)))]))

;; Examples:
(check-expect (NumR/C grid2) 5)
(check-expect (NumR/C grid1) 3)
(check-expect (NumR/C grid3) 1)

;; (grid-ref T3Grid row column) consumes a T3Grid and a row and column number
;; The function produces the symbol located at that location.
;; Row and column numbers start counting from 0.
;; grid-ref: T3Grid Int Int -> (anyof 'O 'X '_)

;; Examples:
(check-expect (grid-ref grid2 1 2) 'O)
(check-expect (grid-ref grid2 0 0) 'X)

(define (grid-ref T3Grid row column)
  (cond
    [(and (<= 0 row) (<= 0 column)
          (< column (NumR/C T3Grid)) (< row (NumR/C T3Grid)))
     (list-ref (list-ref T3Grid row) column)]))

;; Tests:
(check-expect (grid-ref grid2 2 4) '_)
(check-expect (grid-ref grid2 4 4) 'X)

;; (c)

;; (col T3Grid column) consumes a T3Grid, and a column number
;; The function produces a list of the symbols in that column without making sure the valid
;; of the column

(define (col T3Grid column)
  (cond
    [(empty? T3Grid) empty]
    [else (cons (list-ref (first T3Grid) column) (col (rest T3Grid) column))]))

;; Examples:

(check-expect (col grid2 0) (list 'X 'X '_ '_ '_))
(check-expect (col grid2 2) (list 'O 'O '_ '_ '_))

;; (get-column T3Grid column) consumes a T3Grid, and a column number
;; The function produces a list of the symbols in that column
;; get-column: T3Grid Int -> (listof Sym) 

;; Example:

(check-expect (get-column grid1 0) (list '_ '_ '_))

(define (get-column T3Grid column)
  (cond
    [(and (<= 0 column) (< column (NumR/C T3Grid)))
     (col T3Grid column)]))

;; Tests:

(check-expect (get-column grid2 1) (list 'O 'X '_ '_ '_))
(check-expect (get-column grid3 0) (list 'X))
         
;; (d)

;; check if a row can satisfy the requirements that a player will win

(define (check-rows row column player)
  (cond
    [(empty? row) true]
    [(= column 0) (cond
                    [(symbol=? (first row) '_) (check-rows (rest row) (- column 1) player)]
                    [else false])]
    [(symbol=? (first row) player)
     (check-rows (rest row) (- column 1) player)]
    [else false]))

;; Examples:

(check-expect (check-rows (list 'X 'X '_) 2 'X) true)
(check-expect (check-rows (list 'X 'X 'O 'X) 2 'X) false)
(check-expect (check-rows (list 'X '_ 'X 'X) 1 'X) true)

;; (will-win? T3Grid row column player) consumes
;; a T3Grid, a row number, a column number, and a player (either 'X or 'O).
;; The function produces true if that player would win by placing a marker at the given location,
;; and false otherwise.
;; Note that if the given location is not blank, then the player will not win.
;; This function does not check the diagonals, only rows and columns.
;; will-win?: T3Grid Int Int (anyof 'X 'O) -> Bool

;; Examples:

(check-expect (will-win? (list (list 'X 'X '_)
                               (list 'O 'X 'O)
                               (list 'O '_ '_)) 0 2 'X) true)

(check-expect (will-win? grid1 0 0 'X) false)

(define (will-win? T3Grid row column player)
  (cond
    [(and (symbol=? (whose-turn T3Grid) player)
          (symbol=? (grid-ref T3Grid row column) '_))
     (or (check-rows (list-ref T3Grid row) column player)
         (check-rows (get-column T3Grid column) row player))]
    [else false]))

;; Tests:

(check-expect (will-win? grid2 0 1 'X) false)

(check-expect (will-win? (list (list 'X 'X '_)
                               (list 'O 'O '_)
                               (list '_ 'X '_)) 1 2 'O) true)

(check-expect (will-win? (list (list 'O 'X '_)
                               (list 'O 'X 'X)
                               (list '_ '_ '_)) 2 0 'O) true)

(check-expect (will-win? (list (list 'O 'X '_)
                               (list 'O 'X '_)
                               (list 'X '_ '_)) 2 0 'O) false)

(check-expect (will-win? (list (list 'O 'X '_)
                               (list 'O 'X '_)
                               (list '_ '_ '_)) 2 0 'O) false)

(check-expect (will-win? (list (list 'X 'X '_)
                               (list 'O 'O 'X)
                               (list '_ '_ '_)) 1 2 'O) false)
