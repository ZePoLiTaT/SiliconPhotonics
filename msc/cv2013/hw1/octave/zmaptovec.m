#######################################################
## usage: zorig = zmaptovec(ix1dorig, ztransf, lsup)
##
## Uses the index vector with the equivalent positions
## of the original image to map the corresponding
## rgb values into the zorig vector

## Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
## Created: 
## Version: 0.0.1
#######################################################
 
function zorig = zmaptovec(ix1dorig, ztransf, lsup)

    %Create new image in 1dvec form
    zorig = ones(size(ztransf))*255;

    %Find valid position of the index
    validpos = find(ix1dorig>0 & ix1dorig<=lsup);

    %Assign RGB channel of the transformed image (x',y') into the equivalent original image (x,y)
    zorig(validpos,:) = ztransf(ix1dorig(validpos),:);

end
