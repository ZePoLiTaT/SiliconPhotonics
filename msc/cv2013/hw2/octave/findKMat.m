%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage: [K, l] = findKMat(p)
%
% Find K matrix using two sets of lines
% that are known to be orthogonal after transformation.
% The first 3 points of parameter p define the first 2 orth lines
% and the lasts 3 points the other 2 orth lines.
% These two constraints are plugged into the equation
% (l1'm1', l1'm2'+l2'm1', l2'm2') S = 0.
% where S = KK'.
% Then the cholesky decomposition is used to extract
% K from S.

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
function [K, l] = findKMat(p)
    l = [cross(p(:,1) , p(:,2))  cross(p(:,2) , p(:,3)) ...
	 cross(p(:,4) , p(:,5))  cross(p(:,5) , p(:,6))];
    l ./ l(3,:);

    % Solve the eq system given by:
    A = [l(1,1)*l(1,2) l(1,1)*l(2,2)+l(2,1)*l(1,2) l(2,1)*l(2,2);
	 l(1,3)*l(1,4) l(1,3)*l(2,4)+l(2,3)*l(1,4) l(2,3)*l(2,4)];

    %[U D V] = svd(A);
    %s = V(3,:);
    ZA = A(1:2,1:2);
    ZB = -A(:,3);
    s = [ZA\ZB;1];
    S=[s(1) s(2);s(2) s(3)];
    K=chol(S, 'lower');
end
