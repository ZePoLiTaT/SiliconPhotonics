%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage: [im, imat, W, H] = zimread(fname)
%
% Loads an image named 'fname' and returns:
% * vector with 3 columns RGB
% * matrix image

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [im, imat, W, H] = zimread(fname)

    imat = imread(fname);
  
    imsize = size(imat);

    H = imsize(1);
    W = imsize(2);

    %Validate number of color channels in the image
    if(size(imsize,2)==2)
        DIMS=1;
    else
	DIMS=imsize(3);
    end

    im = reshape(imat,W*H,DIMS);
    %im = [ vec(imat(:,:,1)), vec(imat(:,:,2)), vec(imat(:,:,3)) ];
end
