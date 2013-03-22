%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
% usage: 
%
% Finds the corresponding original no transformed
% image points (x,y) from the transformed (x',y') ones

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
 
function [iPos] = transformImg(Hp, fBounds)

    %Generate the (x,y) grid of positions for the final image:
    %   [x1 x2 .. xn]
    %   [y1 y2 .. yn]
    %   [ 1  1 ..  1]
    [xx,yy]=meshgrid([fBounds(1):fBounds(3)],[fBounds(2):fBounds(4)]);
    ptransf=[xx(:)';yy(:)'];

    %Transforms all points
    iPos = transformX(Hp, ptransf);
end
