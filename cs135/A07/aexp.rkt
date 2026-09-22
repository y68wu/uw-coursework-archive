;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname aexp) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 07
;;*******************************************
;;

;; ************ Q2a *************

;; An Arithmetic Expression (AExp) is one of:
;; * (anyof 'x 'y 'z)
;; * Num
;; * OpNode

(define-struct opnode (op args))
;; An Operator Node (OpNode) is a
;; (make-opnode (anyof '* '+) (listof AExp))

;; ************ Q2b *************

;; aexp-template: AExp -> Any
(define (aexp-template ex)
  (cond
    [(number? ex) ...]
    [(symbol? ex) ...]
    [(opnode? ex) (opnode-template (opnode-op ex)
                                   (opnode-args ex))]))

;; opnode-template: (anyof '* '+) (listof AExp) -> Any
(define (opnode-template op args)
  (cond
    [(empty? args) ...]
    [else (... (aexp-template (first args))
               (opnode-template op  (rest args)))]))

;; ************ Q2c *************

(define (key kv) (first kv))
(define (val kv) (second kv))

;; (lookup-al k alst) produces the value corresponding to key k, or false if k not present.

(define (lookup-al k alst)
  (cond
    [(empty? alst) false]
    [(symbol=? k (key (first alst))) (val (first alst))]
    [else (lookup-al k (rest alst))]))

(define sym-table (list (list 'x 1) (list 'y 2) (list 'z 4)))

;; use: (anyof '+ '*) (listof AExp) → Num
(define (use op args sym-table)
  (cond
    [(empty? args)
     (cond [(symbol=? op '+) 0]
           [(symbol=? op '*) 1])]
    [(symbol=? op '+) (+ (eval (first args) sym-table)
                         (use op (rest args) sym-table))]
    [(symbol=? op '*) (* (eval (first args) sym-table)
                         (use op (rest args) sym-table))]))

;; (eval exp sym-table) consumes two parameters exp and a sym-table
;; The function produces the arithemetic expression of exp
;; eval: AExp (listof any) -> Num

;; Example:

(define a-exp (make-opnode '+ (list 'x 'y (make-opnode '* (list 5 'z)))))

(check-expect (eval a-exp sym-table) 23)

(define (eval exp sym-table)
  (cond
    [(number? exp) exp]
    [(symbol? exp) (lookup-al exp sym-table)]
    [(opnode? exp) (use (opnode-op exp)
                          (opnode-args exp)
                          sym-table)]))

;; Test:

(define a-exp2 (make-opnode '+ (list 'x 'y (make-opnode '* (list 7 'z)))))

(check-expect (eval a-exp2 sym-table) 31)

;; ************ Q2d *************

;; (maybe-simplify-constant op lst) consumes an operator and a list of arithmetic expressions.
;; The function produces a number or an opcode depending on the list.
;; maybe-simplify-constant: op (listof any) -> (listof any)

;; Example:

(check-expect (maybe-simplify-constant '+ (list 9)) 9)

(define (maybe-simplify-constant op lst)
  (cond
    [(= (length lst) 1) (first lst)]
    [else (make-opnode op lst)]))

;; Tests:

(check-expect (maybe-simplify-constant '+ (list 'y)) 'y)
(check-expect (maybe-simplify-constant '+ (list 'x 10 'y)) (make-opnode '+ (list 'x 10 'y)))

;; (simplify/combine op arg1 other-args acc) consumes an operator, an arithmetic expression,
;; a list of arithmetic expressions,and an accumulator.
;; The function produces a new list of arithmetic expressions by adding it to the list
;; if it is a symbol or opnode, otherwise update the accumulator accordingly.
;; simplify/combine: op (anyof 'x 'y 'z opnode Num) (listof args) Num -> (listof any)

;; Example:

(check-expect (simplify/combine '+ 'y (list 'x) 0) (list 'y 'x))

(define (simplify/combine op arg1 other-args acc)
  (cond
    [(empty? other-args) (cond
                           [(and (symbol=? op '+) (number? arg1)) (list (+ arg1 acc))]
                           [(and (symbol=? op '*) (number? arg1)) (list (* arg1 acc))]
                           [(and (symbol? arg1) (= acc 0)) (list arg1)]
                           [else (cons arg1 (list acc))])]
    [(number? arg1) (cond
                      [(symbol=? op '+)
                       (simplify/combine op (first other-args) (rest other-args) (+ arg1 acc))]
                      [(and (symbol=? op '*) (= acc 0))
                       (simplify/combine op (first other-args) (rest other-args) (* arg1 1))]
                      [else
                       (simplify/combine op (first other-args) (rest other-args) (* arg1 acc))])]
    [(symbol? arg1)
     (cons arg1 (simplify/combine op (first other-args) (rest other-args) acc))]
    [(and (opnode? arg1) (or (number? (simplify arg1)) (symbol? (simplify arg1))))
     (simplify/combine op (simplify arg1) other-args acc)]
    [else
      (cons (simplify arg1) (simplify/combine op (first other-args) (rest other-args) acc))]))

;; Test:

(check-expect (simplify/combine '+ 5 (list 'x) 0) (list 'x 5))

;; (simplify/lst op args acc) consumes an operator, a list of arithmetic ex- pressions,
;; and an accumulator. The function produces the simplified list of arguments. 
;; simplify/lst: op args num -> (listof any)

;; Example:
(check-expect (simplify/lst '+ (list 6 'x (make-opnode '* (list 1 2 3))) 0)
              (list 'x (make-opnode '* (list 1 2 3)) 6))

(define (simplify/lst op args acc)
  (cond
    [(empty? args) acc]
    [else (simplify/combine op (first args) (rest args) acc)]))

;; Test:
(check-expect (simplify/lst '+ (list 6 'x 7) 0) (list 'x 13))

;;(simplify ex) consumes an AExp and produces an equivalent AExp which has been simplified
;; simplify: AExp -> AExp

;; Examples:

(check-expect (simplify (make-opnode '* (list 2 'x (make-opnode '+ (list 3 3)) 'z 6)))
              (make-opnode '* (list 'x 'z 72)))
(check-expect (simplify (make-opnode '* (list 2 'x (make-opnode '+ (list 3 'x 3)) 'z 6)))
              (make-opnode '* (list 'x (make-opnode '+ (list 'x 6)) 'z 12)))

(define (simplify ex)
  (cond
    [(number? ex) ex]
    [(symbol? ex) ex]
    [(opnode? ex)
    (maybe-simplify-constant (opnode-op ex) (simplify/lst (opnode-op ex) (opnode-args ex) 0))]))


;; Tests:
(check-expect (simplify 6) 6)
(check-expect (simplify 'y) 'y)
(check-expect (simplify (make-opnode '* (list 2 3 1))) 6)
(check-expect(simplify (make-opnode '* (list 2 'x (make-opnode '+ (list 'x)) 'z 6)))
             (make-opnode '* (list 'x 'x 'z 12)))