## usage: mat = zimasmat(vec, w, h)
##
## Transforms an image stored in a vector into a 
## matrix [h,w,3] that can be shown with imshow

## Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
## Created: 
## Version: 0.0.1 

function imat = zimasmat(imvec, w, h)

    imat = ones(h,w,3);
    for i = 1:3
        imat(:,:,i) = reshape(imvec(:,i),h,w);
    end 
    imat = uint8(imat); 
end
