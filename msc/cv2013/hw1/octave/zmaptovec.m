function zorig = zmaptovec(ix1dorig, ztransf, lsup)

    %Create new image in 1dvec form
    zorig = ones(size(ztransf))*255;

    %Find valid position of the index
    validpos = find(ix1dorig>0 & ix1dorig<=lsup);

    %Assign RGB channel of the transformed image (x',y') into the equivalent original image (x,y)
    %zorig(ix1dorig>0,:) = ztransf(find(ix1dorig>0),:);
    zorig(validpos,:) = ztransf(ix1dorig(validpos),:);

end
