%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage:
%
% ...
% ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [fImg, fVecImg] = createTransfImg(fSize, iSize, iPos, iVecImg, interp)
    if (nargin < 5)  ||  isempty(interp)
        interp = 'nearest';
    end

    % find valid position of the indexes refering to the original image
    iX = find(iPos(:,1)>=2 & iPos(:,2)>=2 & iPos(:,1)<=(iSize(1)-2) & iPos(:,2)<=(iSize(2)-2));
    % find the the indexes (ix) of fPos from the unwrapped vector iPos
    fPos = [1:size(iPos,1)]';

    % create original image of size fWxfH
    fVecImg = zeros(fSize(2)*fSize(1), 3);

    % assign the specific interpolation from the rgb channel of the transformed 
    % image (x',y') into the equivalent original image (x,y)
    fprintf('%s interpolation\n',interp);
    switch (interp)
        case 'bilinear'
            fVecImg(fPos(iX,:),:) = bilinear(iVecImg, iPos(iX,:), iSize(2));
        case 'bicubic'
            fVecImg(fPos(iX,:),:) = bicubic(iVecImg, iPos(iX,:), iSize(2));
        otherwise %nearest
            fVecImg(fPos(iX,:),:) = nearestN(iVecImg, iPos(iX,:), iSize(2));
    end

    % Transform vector to matrix image
    fImg = zimasmat(fVecImg, fSize(1), fSize(2));
end
