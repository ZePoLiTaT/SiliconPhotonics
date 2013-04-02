%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage: [VL,l] = findVL(A,B,C,D)
%
% Returns the vanishing line from 4 points that 
% belong to the corners (clockwise order) of a 
% rectangle when when the projective transformation 
% is removed

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: Apr 1 2013
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [VL,l] = findVL(A,B,C,D)
    l = [cross(A,B) cross(C,D) cross(A,C) cross(B,D)];
    P = cross( l(:,1) , l(:,2) );
    Q = cross( l(:,3) , l(:,4) );
    VL = cross(P,Q);    
    VL = VL ./ VL(3);
end
