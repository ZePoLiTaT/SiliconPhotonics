;--------------------------Parameters----------------------------
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

(define fill_mat air);
(define sio2_mat (make dielectric (index 1.44427))) 
(define si_mat (make dielectric (index 3.476558)))

(set! default-material fill_mat)

;--------------------------Geometry----------------------------
(set! geometry-lattice (make lattice (size sxy sxy no-size)))
(set! geometry (list
	;Outer ring
	(make cylinder 
	    (center 0 0) (height infinity) (radius (+ r w)) 
	    (material si_mat))
	;Inner ring 
	(make cylinder 
	    (center 0 0) (height infinity) (radius r) 
	    (material fill_mat))
	;Upper waveguide
	(make block 
	    (center 0 (+ r w gap (/ w 2))) (size sxy w infinity)
	    (material si_mat))
	;Bottom waveguide
	(make block 
	    (center 0 (* -1 (+ r w gap (/ w 2)))) (size sxy w infinity)
	    (material si_mat))
))

(set-param! resolution 80)
(set! pml-layers (list (make pml (thickness dpml))))

;--------------------------Sources----------------------------
(define theta (/ pi 4))
(define kx (* freqcen (sin theta)))
(set! k-point (vector3 kx 0 0))
(set! ensure-periodicity true)
(define (my-amp-func p) 
    (exp (* 0+2i pi kx (vector3-x p))))

(set! sources (list
  (make source
    (src (make gaussian-src (frequency freqcen) (fwidth (* 3 freq_width)) ))
    (component Ez)
    (size 0 w)
;    (amp-func my-amp-func)
    (center (+ dpml (* -0.5 sxy)) (+ r w gap (/ w 2)) ) )
))
 
;--------------------------Fluxes----------------------------
;in flux
(define influx 		 
    (add-flux freqcen freq_width flux_points
       	(make flux-region
	   	(center (+ 1 dpml (* -0.5 sxy)) (+ r w gap (/ w 2))) 
            	(size 0 (* 2 w)) )))
;through flux
(define thflux		
    (add-flux freqcen freq_width flux_points
	(make flux-region 
		(center (- (* 0.5 sxy) dpml 1) (+ r w gap (/ w 2))) 
		(size 0 (* 2 w))  )))
;drop flux
(define drflux 		 
    (add-flux freqcen freq_width flux_points
       	(make flux-region
	   	(center (+ 1 dpml (* -0.5 sxy)) (* -1 (+ r w gap (/ w 2)))) 
            	(size 0 (* 2 w)) )))

;--------------------------Simulation----------------------------
(use-output-directory (string-append odir (get-filename-prefix)))
(run-sources+ 
    (stop-when-fields-decayed 50 Ez (vector3 (- (* 0.5 sxy) dpml 1) (+ r w gap (/ w 2))) 1e-3)
    (at-beginning output-epsilon)
    (at-end output-efield-z)
;    (at-every 40 (output-png Ez "-S3 -Zc dkbluered -a yarg -A $EPS"))
)

(display-fluxes influx thflux drflux)

