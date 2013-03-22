%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
% usage: vTransformed = zfindX(Hp, W, H)
%
% Finds the corresponding original no transformed
% image points (x,y) from the transformed (x',y') ones

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
 
function [iPos] = transformX(Hp, iSize, fBounds)

    %Generate matrix of points in transformed image:
    %   [x1 x2 .. xn]
    %   [y1 y2 .. yn]
    %   [ 1  1 ..  1]
    [xx,yy]=meshgrid([fBounds(1):fBounds(3)],[fBounds(2):fBounds(4)]);
    ptransf=[xx(:)';yy(:)';ones(size(xx(:)'))];

    %Find equivalent points in original (no transformed) image by
    %using the inverse of the transformation matrix Hp
    iPos = Hp * double(ptransf);

    %Normalize x1 and x2 by x3. 
    iPos = bsxfun(@rdivide, iPos(1:2,:), iPos(3,:))';
   
end
