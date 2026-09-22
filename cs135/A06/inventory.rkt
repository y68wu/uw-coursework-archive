;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname inventory) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;
;;*******************************************
;;Chris Wu
;;CS 135 Winter 2023
;;Assignment 06
;;*******************************************
;;


;; ************* Q2a **************

(define-struct ev (model year price mileage mpge))

;; Data Definition:
;; An EV is a: (make-ev Str Nat Num Num Num)
;; Requires: price >= 0, mileage >= 0, mpge >= 0

;; ************* Q2b **************

(define evlst1 (list (make-ev "BMW" 2003 1000 1000 132)
                     (make-ev "Benz" 2015 2000 900 93)))

;; (adjust-prices evlst pc) consumes a list of electric vehicles and a number
;; representing a percent change. 
;; The function produces a list with the same vehicles such that the price of each vehicle has been
;; lowered or raised by the amount given.
;; adjust-prices: (listof EV) num -> (listof EV)

;; Example:

(check-expect (adjust-prices evlst1 0.10) (list (make-ev "BMW" 2003 1100 1000 132)
                                                (make-ev "Benz" 2015 2200 900 93)))

(define (adjust-prices evlst pc)
  (cond
    [(empty? evlst) empty]
    [else (cons (make-ev (ev-model (first evlst))
                         (ev-year (first evlst))
                         (+ (ev-price (first evlst)) (* (ev-price (first evlst)) pc))
                         (ev-mileage (first evlst))
                         (ev-mpge (first evlst)))
                 (adjust-prices (rest evlst) pc))]))

;; Tests:

(check-expect (adjust-prices evlst1 0) (list (make-ev "BMW" 2003 1000 1000 132)
                                             (make-ev "Benz" 2015 2000 900 93)))

(check-expect (adjust-prices evlst1 -0.10) (list (make-ev "BMW" 2003 900 1000 132)
                                                 (make-ev "Benz" 2015 1800 900 93)))

;; ************* Q2c **************

(define models (list "Hyundai Kona" "Audi e-Tron" "Chevy Bolt" "BMW i4" "Tesla M3" "Nissan Leaf"))
(define years (list 2022 2021 2020 2022 2022 2013))
(define prices (list 36000 40000 35000 53000 60000 5000))
(define mileage (list 12000 4000 3050 25 5 230000))
(define mpge (list 132 63 123 75 133 98))

;; Example:

(check-expect (acc-build-inventory models years prices mileage mpge
                                   (list (make-ev "BMW" 2003 1000 1000 132)))
              (list (make-ev "Nissan Leaf" 2013 5000 230000 98)
                    (make-ev "Tesla M3" 2022 60000 5 133)
                    (make-ev "BMW i4" 2022 53000 25 75)
                    (make-ev "Chevy Bolt" 2020 35000 3050 123)
                    (make-ev "Audi e-Tron" 2021 40000 4000 63)
                    (make-ev "Hyundai Kona" 2022 36000 12000 132)
                    (make-ev "BMW" 2003 1000 1000 132)))

(define (acc-build-inventory models years prices mileage mpge acclst)
  (cond
    [(empty? models) acclst]
    [else (acc-build-inventory (rest models) (rest years) (rest prices) (rest mileage) (rest mpge)
                               (cons (make-ev (first models)
                                              (first years)
                                              (first prices)
                                              (first mileage)
                                              (first mpge)) acclst))]))


;; Test:

(check-expect (acc-build-inventory models years prices mileage mpge empty)
              (list (make-ev "Nissan Leaf" 2013 5000 230000 98)
                    (make-ev "Tesla M3" 2022 60000 5 133)
                    (make-ev "BMW i4" 2022 53000 25 75)
                    (make-ev "Chevy Bolt" 2020 35000 3050 123)
                    (make-ev "Audi e-Tron" 2021 40000 4000 63)
                    (make-ev "Hyundai Kona" 2022 36000 12000 132)))


;; (build-inventory models years prices mileage mpge). Each parameter is a list;
;; all the lists are the same length.
;; The ith value in each list is an attribute of the ith car in the inventory
;; (build-inventory models years prices mileage mpge) produces a list of EVs with the same information
;; build-inventory: (listof models) (listof years) (listof prices) (listof mileage) (listof mpge) -> (listof EV)

;; Example:

(check-expect (build-inventory models years prices mileage mpge)
              (list (make-ev "Nissan Leaf" 2013 5000 230000 98)
                    (make-ev "Tesla M3" 2022 60000 5 133)
                    (make-ev "BMW i4" 2022 53000 25 75)
                    (make-ev "Chevy Bolt" 2020 35000 3050 123)
                    (make-ev "Audi e-Tron" 2021 40000 4000 63)
                    (make-ev "Hyundai Kona" 2022 36000 12000 132)))

(define (build-inventory models years prices mileage mpge)
  (acc-build-inventory models years prices mileage mpge empty))

;; Test:

(check-expect (build-inventory models years prices mileage (list 1 2 3 4 5 6))
              (list (make-ev "Nissan Leaf" 2013 5000 230000 6)
                    (make-ev "Tesla M3" 2022 60000 5 5)
                    (make-ev "BMW i4" 2022 53000 25 4)
                    (make-ev "Chevy Bolt" 2020 35000 3050 3)
                    (make-ev "Audi e-Tron" 2021 40000 4000 2)
                    (make-ev "Hyundai Kona" 2022 36000 12000 1)))

;; ************* Q2d **************

;; (compare-ev EV1 EV2) consumes two EVs
;; The function produces 'lt if the first one is “less than” the second,'eq if they are equal,
;; and 'gt if the first one is “greater than” the second.
;; The comparisons should be based on year, mpge, and mileage. Older cars are “less than” newer cars.
;; If they are the same age, cars with a lower efficiency (mpge)
;; are “less than” cars with higher efficency.
;; If they are the same age and efficiency,cars with more miles are “less than” cars with fewer miles.
;; compare-ev: EV EV -> (Anyof 'lt 'gt 'eq)

;; Example:

(check-expect (compare-ev (make-ev "Tesla M3" 2022 60000 5 5)
                          (make-ev "Tesla M3" 2022 60000 5 5)) 'eq)

(check-expect (compare-ev (make-ev "Tesla" 2022 40000 5 5)
                          (make-ev "BMWX7" 2022 60000 10 5)) 'gt)

(define (compare-ev EV1 EV2)
  (cond
    [(< (ev-year EV1) (ev-year EV2)) 'lt]
    [(< (ev-year EV2) (ev-year EV1)) 'gt]
    [(< (ev-mpge EV1) (ev-mpge EV2)) 'lt]
    [(< (ev-mpge EV2) (ev-mpge EV1)) 'gt]
    [(> (ev-mileage EV1) (ev-mileage EV2)) 'lt]
    [(> (ev-mileage EV2) (ev-mileage EV1)) 'gt]
    [else 'eq]))

;; Tests:

(check-expect (compare-ev (make-ev "Tesla" 2022 60000 5 5)
                          (make-ev "BMWX7" 2019 60000 5 5)) 'gt)

(check-expect (compare-ev (make-ev "Tesla" 2022 60000 5 5)
                          (make-ev "BMWX7" 2022 60000 1000 10)) 'lt)


;; ************* Q2e **************

(define (biggest-first/acc evlst acc acclst)
  (cond
    [(empty? evlst) acclst]
    [(or (symbol=? (compare-ev (first evlst) acc) 'gt) (symbol=? (compare-ev (first evlst) acc) 'eq))
     (biggest-first/acc (rest evlst) (first evlst) (cons (first evlst) acclst))]
    [else (biggest-first/acc (rest evlst) acc (append acclst (list (first evlst))))]))

(define (biggest-first evlst)
  (biggest-first/acc evlst (first evlst) empty))

(define (sel-sort/sf evlst)
  (cond
    [(empty? (rest evlst)) (cons (first evlst) empty)]
    [else (cons (first evlst) (sel-sort/sf (biggest-first (rest evlst))))]))

;; (sort-evs evlst) that consumes a list of electric vehicles
;; The function produces an inventory list in the following order:
;; i. year (descending order) ii. mpge (descending order) iii. mileage (ascending order)
;; If two EVs compare equal, their order in the resulting list does not matter.
;; sort-evs: (listof EV) -> (listof EV)

;; Test:

(check-expect (sort-evs (list (make-ev "Nissan Leaf" 2013 5000 230000 6)
                    (make-ev "Tesla M3" 2022 60000 5 5)
                    (make-ev "BMW i4" 2022 53000 25 4)
                    (make-ev "Chevy Bolt" 2020 35000 3050 3)
                    (make-ev "Audi e-Tron" 2021 40000 4000 2)
                    (make-ev "Hyundai Kona" 2022 36000 12000 1)))
              (list
               (make-ev "Tesla M3" 2022 60000 5 5)
               (make-ev "BMW i4" 2022 53000 25 4)
               (make-ev "Hyundai Kona" 2022 36000 12000 1)
               (make-ev "Audi e-Tron" 2021 40000 4000 2)
               (make-ev "Chevy Bolt" 2020 35000 3050 3)
               (make-ev "Nissan Leaf" 2013 5000 230000 6)))

(define (sort-evs evlst)
  (cond
    [(empty? evlst) empty]
    [else (sel-sort/sf (biggest-first evlst))]))


(check-expect (sort-evs (list (make-ev "Nissan Leaf" 2022 5000 230000 6)
                    (make-ev "Tesla M3" 2022 60000 5 5)
                    (make-ev "BMW i4" 2022 53000 25 4)
                    (make-ev "Chevy Bolt" 2022 35000 3050 4)
                    (make-ev "Audi e-Tron" 2022 40000 4000 4)
                    (make-ev "Hyundai Kona" 2022 36000 12000 1)))
              (list
               (make-ev "Nissan Leaf" 2022 5000 230000 6)
               (make-ev "Tesla M3" 2022 60000 5 5)
               (make-ev "BMW i4" 2022 53000 25 4)
               (make-ev "Chevy Bolt" 2022 35000 3050 4)
               (make-ev "Audi e-Tron" 2022 40000 4000 4)
               (make-ev "Hyundai Kona" 2022 36000 12000 1)))