;Units are measured in microns [a=1uc]
(define-param odir "./out-") 
(define-param w 0.4)	; width of waveguide         
(define-param r 2.9)	; inner radius of ring
(define-param gap 0.1)	; gap between ring and wg
(define-param pad 4)	; padding between waveguide and edge of PML
(define-param dpml 2)	; thickness of PML
(define-param wavecen 1.550)
(define-param wavewid 0.050)
(define-param freqcen (/ 1 wavecen))
(define-param freq_width (- (/ 1 (- wavecen wavewid)) (/ 1  (+ wavecen wavewid))))
(define-param flux_points 100)

(define sxy (* 2 (+ r w pad dpml))) ; cell size

(define sio2_mat (make dielectric
    (index 1)))
;    (index 1.44427))) 
;    (index 1.97926701840823)(D-conductivity (/ (* 2 pi freqcen 0) 3.9174979301586 ))))
(define si_mat (make dielectric 
    (index 3.476558)))
;    (index 3.47747760586317)(D-conductivity (/ (* 2 pi freqcen 0) 12.0928504992798 ))))

(set! default-material sio2_mat)

(set! geometry-lattice (make lattice (size sxy sxy no-size)))
(set! geometry (list
    (make cylinder 
        (center 0 0) (height infinity) (radius (+ r w)) 
        (material si_mat))
    (make cylinder 
        (center 0 0) (height infinity) (radius r) 
        (material sio2_mat))
    (make block 
        (center 0 (+ r w gap (/ w 2))) (size sxy w infinity)
        (material si_mat))))

(set-param! resolution 35)

(define theta (/ pi 4))
(define kx (* freqcen (sin theta)))
(set! k-point (vector3 kx 0 0))
(set! ensure-periodicity true)
(define (my-amp-func p) 
    (exp (* 0+2i pi kx (vector3-x p))))

(set! pml-layers (list (make pml (thickness dpml))))

(set! sources (list
  (make source
    (src (make continuous-src (frequency freqcen) ))
    (component Ez)
    (size 0 w)
;    (amp-func my-amp-func)
    (center (+ dpml (* -0.5 sxy)) (+ r w gap (/ w 2)) ) )
))
 
;transmitted flux
(define iniflux 		 
    (add-flux freqcen freq_width flux_points
       	(make flux-region
	   	(center (+ 1 dpml (* -0.5 sxy)) (+ r w gap (/ w 2))) 
            	(size 0 (* 2 w)) )))
;reflected flux
(define endflux		
    (add-flux freqcen freq_width flux_points
	(make flux-region 
		(center (- (* 0.5 sxy) dpml 1) (+ r w gap (/ w 2))) 
		(size 0 (* 2 w))  )))

(use-output-directory (string-append odir (get-filename-prefix)))
(run-until 3000 
    (at-beginning output-epsilon)
    (at-end output-efield-z)
    (at-every 40 (output-png Ez "-S3 -Zc dkbluered -a yarg -A $EPS"))
)

(display-fluxes iniflux endflux)

