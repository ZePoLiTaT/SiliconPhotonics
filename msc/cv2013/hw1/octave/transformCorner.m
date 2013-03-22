%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage:
%
% Finds the transformation of the corners to get the new size of the image

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
function [fSize, fBounds] = transformCorner(Hp, iSize)

    pCorner = [ 1 iSize(1,1) iSize(1,1) 1 ;
		1 1 iSize(1,2) iSize(1,2);
		ones(1,4) ];

    pTransCorner = Hp * pCorner;
    pTransCorner = bsxfun(@rdivide, pTransCorner(1:2,:), pTransCorner(3,:));
    pTransCorner = int64(pTransCorner)';

    %Calculate the size of the new image
    %tmp=num2cell([min(pTransCorner) max(pTransCorner)]);
    %[minx miny maxx maxy]= tmp{:};

    %fBounds = [minx miny maxx maxy]
    fBounds=[min(pTransCorner) max(pTransCorner)];
    fSize = [fBounds(3)-fBounds(1)+1 , fBounds(4)-fBounds(2)+1];
end

