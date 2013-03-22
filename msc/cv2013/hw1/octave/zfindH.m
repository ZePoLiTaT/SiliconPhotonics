%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage: Hmat = zfindH(x,y,xp,yp) 
%
% Finds the tranformation matrix H based on
% the 4 points in the projected image [x,y] and
% the 4 points in the original image [xp,yp]

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function  Hmat = zfindH(x,y,xp,yp) 

    A = zeros(8,8);
    B = zeros(8,1);

    for i=1:4
	A(2*i-1,:)=[x(i) y(i) 1 0 0 0 -xp(i)*x(i) -xp(i)*y(i)];
	A(2*i,:) = [0 0 0 x(i) y(i) 1 -yp(i)*x(i) -yp(i)*y(i)];

	B(2*i-1) = xp(i);
	B(2*i) = yp(i);
    end

    Hmat = A\B;
    Hmat = reshape([Hmat;1],3,3)';

end
