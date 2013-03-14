## usage: [im, imat, w, h] = zimread(fname)
##
## Loads an image named 'fname' and returns:
## * vector with 3 columns RGB
## * matrix image

## Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
## Created: 
## Version: 0.0.1

function [im, imat, w, h] = zimread(fname)

    imat = imread(strcat(fname));
  
    h = size(imat)(1);
    w = size(imat)(2);
    im = [ vec(imat(:,:,1)), vec(imat(:,:,2)), vec(imat(:,:,3)) ];

end
