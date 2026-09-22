;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname collide) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;;
;*******************************************
;Chris Wu
;CS 135 Winter 2023
;Assignment 03
;*******************************************
;;

;;Q3

; Q3a
;; *********** (a) *************

;; (define p1 (cons 0 (cons 1 (cons 2 empty))))

;; define x-value of a point
(define (x-value point)
  (first point))

;; define y-value of a point
(define (y-value point)
  (first (rest point)))

;; define z-value of a point
(define (z-value point)
  (first (rest (rest point))))

;; (distance-between-points point1 point2) consumes two parameter: point1 point2
;; produces the distance between two points
;; distance-between-points: point point -> Num
;; (check-expect (distance-between-points point point) Num)

;; Examples:
(define p1 (cons 0 (cons 1 (cons 2 empty))))
(define p2 (cons 1 (cons 3 (cons 4 empty))))
(define p3 (cons 2 (cons 5 (cons 6 empty))))
(check-expect (distance-between-points p1 p2) 3)
(check-expect (distance-between-points p2 p3) 3)

(define (distance-between-points
         point1
         point2)
  (sqrt (+ (sqr (- (x-value point2) (x-value point1)))
           (sqr (- (y-value point2) (y-value point1)))
           (sqr (- (z-value point2) (z-value point1))))))

;; test:
(define p4 (cons 2 (cons 4 (cons 8 empty))))
(check-expect (distance-between-points p1 p4) 7)

;; Q3b
;; *********** (b) ************
;; A Point is a (cons Num (cons Num (cons Num empty)))
;; A Sphere is a (cons Point (cons Num empty))

;; x-value of a sphere
(define (cx-value sphere)
  (x-value (first sphere)))

;; y-value of a sphere
(define (cy-value sphere)
  (y-value (first sphere)))

;; z-value of a sphere
(define (cz-value sphere)
  (z-value (first sphere)))

;; sphere's radius
(define (radius sphere)
  (first (rest sphere)))

;; (point-in-sphere? point sphere) consumes two parameter: point sphere
;; The function produces true if the point is inside the sphere and false otherwise.
;; point-in-sphere?: point sphere -> bool
;; (check-expect (point-in-sphere? point sphere) bool)

;; Examples:
;; (define p1 (cons 0 (cons 1 (cons 2 empty))))
;; (define p2 (cons 1 (cons 3 (cons 4 empty))))
;; (define p3 (cons 2 (cons 5 (cons 6 empty))))

(define sphere1 (cons p1 (cons 3 empty)))
(define sphere2 (cons p1 (cons 3 empty)))
(check-expect (point-in-sphere? p1 sphere1) true)
(check-expect (point-in-sphere? p2 sphere1) true)

(define (point-in-sphere?
         point
         sphere)
  (cond
    [(<= (+ (sqr (- (x-value point) (cx-value sphere)))
            (sqr (- (y-value point) (cy-value sphere)))
            (sqr (- (z-value point) (cz-value sphere))))
         (sqr (radius sphere))) true]
    [else false]))

;; Test
(check-expect (point-in-sphere? p1 sphere2) true)
(check-expect (point-in-sphere? p2 sphere2) true)

;; Q3c
;; *********** (c) ************

;; define center point of a sphere
(define (sphere-point
         sphere)
  (first sphere))

;; (collide? sphere1 sphere2) consumes two parameter: sphere1 sphere2
;; The function produces true if the spheres collide and false otherwise.
;; collide?: sphere sphere -> bool
;; (check-expect (point-in-sphere sphere sphere) bool)

;; Examples:
(define p5 (cons -3 (cons -1 (cons 2 empty))))

(define sphere3 (cons p2 (cons 5 empty)))
(define sphere4 (cons p2 (cons 2 empty)))
(define sphere5 (cons p5 (cons 4 empty)))

(check-expect (collide? sphere1 sphere2) true)
(check-expect (collide? sphere1 sphere4) true)

(define (collide?
         sphere1
         sphere2)
  (cond
    [(<= (distance-between-points
          (sphere-point sphere1)
          (sphere-point sphere2))
         (+ (radius sphere1)
            (radius sphere2))) true]
    [else false]))

;; Test:
(check-expect (collide? sphere3 sphere4) true)
(check-expect (collide? sphere2 sphere4) true)
(check-expect (collide? sphere1 sphere5) true)