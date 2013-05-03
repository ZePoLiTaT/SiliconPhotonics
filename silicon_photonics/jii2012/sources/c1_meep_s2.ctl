(define si_mat (make dielectric (index 3.476558)))
(set! default-material air)

(set! geometry-lattice (make lattice (size sxy sxy no-size)))
(set! geometry (list
    (make cylinder 
        (center 0 0) (height infinity) (radius (+ r w)) 
        (material si_mat))
    (make cylinder 
        (center 0 0) (height infinity) (radius r) 
        (material air))
    (make block 
        (center 0 (+ r w gap (/ w 2))) (size sxy w infinity)
        (material si_mat))))

(set-param! resolution 35)

