%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage:
%
% ...
% ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [fImg, fVecImg] = createTransfImg(fSize, iSize, iPos, iVecImg)
    % find valid position of the indexes refering to the original image
    iX = find(iPos(:,1)>=1           & iPos(:,2)>=1 & 
	      iPos(:,1)<(iSize(1)-1) & iPos(:,2)<(iSize(2)-1));
    % find the the indexes (ix) of fPos from the unwrapped vector iPos
    fPos = [1:size(iPos,1)]';

    % create original image of size fWxfH
    fVecImg = zeros(fSize(2)*fSize(1), 3);
    % assign the bilinear interpolation from the rgb channel of the transformed image (x',y') 
    % into the equivalent original image (x,y)
    fVecImg(fPos(iX,:),:) = bilinear(iVecImg, iPos(iX,:), iSize(2));
    % Transform vector to matrix image
    fImg = zimasmat(fVecImg, fSize(1), fSize(2));
end
