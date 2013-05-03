;Units are measured in microns [a=1uc]
(define-param odir "./out-") 
(define-param w 0.4)	; width of waveguide         
(define-param r 2.9)	; inner radius of ring
(define-param gap 0.1)	; gap between ring and wg
(define-param pad 4)	; padding between wg and edge of PML
(define-param dpml 2)	; thickness of PML
(define-param wavecen 1.550)
(define-param wavewid 0.050)
(define-param freqcen (/ 1 wavecen))
(define-param freq_width (- 
    (/ 1 (- wavecen wavewid)) 
    (/ 1  (+ wavecen wavewid))))
(define-param flux_points 100)

(define sxy (* 2 (+ r w pad dpml))) ; cell size
(define si_mat (make dielectric (index 3.476558)))

