;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname components) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 07
;;*******************************************
;;

(define-struct component (name num subcomponents))

(define bike (make-component
              "bike" 1
              (list
               (make-component "frame" 1 empty)
               (make-component "wheel" 2
                               (list
                                (make-component "tire" 1 empty)
                                (make-component "rim" 1 empty)
                                (make-component "spoke" 30 empty)
                                (make-component "hub" 1
                                                (list (make-component "housing" 1 empty)
                                                      (make-component "axel" 1 empty)
                                                      (make-component "bearing" 20
                                                             empty)))))
               (make-component "seat" 1 empty)
               (make-component "handlebar" 1 empty))))

;; contains-sub?: subcomponents Str -> Bool

;; Examples

(check-expect (contains-sub? (component-subcomponents bike) "handlebar") true)
(check-expect (contains-sub? (component-subcomponents bike) "bearing") true)

(define (contains-sub? sub name)
  (cond
    [(empty? sub) false]
    [else (or (contains-component? (first sub) name)
              (contains-sub? (rest sub) name))]))

;; Test:

(check-expect (contains-sub? (component-subcomponents bike) "spoke") true)
(check-expect (contains-sub? (component-subcomponents bike) "chris") false)

;; (contains-component? name) consumes a component and a name
;; The function produces true if the component or one of its subcomponents (recursively)
;; has the given name (and false otherwise).
;; contains-component?: component Str -> Bool

;; Examples:

(check-expect (contains-component? bike "hub") true)
(check-expect (contains-component? bike "brake") false)

(define (contains-component? component name)
  (cond
    [(string=? (component-name component) name) true]
    [else (contains-sub? (component-subcomponents component) name)]))

;; Test:

(check-expect (contains-component? bike "handlebar") true)
(check-expect (contains-component? bike "bearing") true)
(check-expect (contains-component? bike "spoke") true)
(check-expect (contains-component? bike "chris") false)