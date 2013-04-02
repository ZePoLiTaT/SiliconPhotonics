%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage:
%
% Finds the transformation of the corners to get the new size of the image

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
function [fSize, fBounds] = transformCorner(Hp, iSize)

    pCorner = [ iSize(1,1) iSize(1,2) iSize(1,2) iSize(1,1);
		iSize(2,1) iSize(2,1) iSize(2,2) iSize(2,2)];

    pTransCorner = transformX(Hp, pCorner);

    %fBounds = [minx miny maxx maxy]
    fBounds=[min(pTransCorner) max(pTransCorner)];
    fSize = round([fBounds(3)-fBounds(1), fBounds(4)-fBounds(2)])
end

