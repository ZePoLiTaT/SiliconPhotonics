%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
% usage: 
%
% Finds the corresponding original no transformed
% image points (x,y) from the transformed (x',y') ones

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
 
function [xT] = transformX(Hp, x)

    %Adds the homogeneous 3rd parameter
    x = [x; ones(1,size(x,2))];

    %Applies transformation
    xT = Hp * double(x);

    %Normalize x1 and x2 by x3. 
    xT = bsxfun(@rdivide, xT(1:2,:), xT(3,:))';
   
end
