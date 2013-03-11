function n = n_lambda(l)
    l2 = l.^2;
    n = sqrt( 
	((10.6684293*l2)./(l2-0.301516485^2)) + 
	((0.003043475*l2)./(l2-1.13475115^2)) + 
	((1.54133408*l2)./(l2-1104^2)) + 1);
endfunction