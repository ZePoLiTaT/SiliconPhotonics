%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage: vl = findVL(A,B,C,D)
%
% Returns the vanishing line from 4 points that 
% belong to the corners (clockwise order) of a 
% rectangle when when the projective transformation 
% is removed

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: Apr 1 2013
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function VL = findVL(A,B,C,D)
    P = cross( cross(A,B) , cross(C,D) );
    Q = cross( cross(A,C) , cross(B,D) );
    VL = cross(P,Q);    
    VL = VL ./ VL(3);
end
