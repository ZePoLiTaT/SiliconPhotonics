%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage:
%
% ...
% ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
function b = nearestN(f, pos, H)
    
    x = round(pos(:,1));
    y = round(pos(:,2));
    f = double(f);
    
    b = f( (x-1)*H+y ,:); 
end
