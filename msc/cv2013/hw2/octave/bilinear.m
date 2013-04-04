%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage:
%
% ...
% ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
function b = bilinear(f, pos, H)
    
    x = pos(:,1);
    y = pos(:,2);
    x1 = floor(x);
    x2 = x1 + 1;
    y1 = floor(y);
    y2 = y1 + 1 ;
    f = double(f);
    
    fQ11 = f( (x1-1)*H + y1,:); 
    fQ21 = f( (x2-1)*H + y1,:);
    fQ12 = f( (x1-1)*H + y2,:);
    fQ22 = f( (x2-1)*H + y2,:);

    b = bsxfun(@times, fQ11, (x2-x).*(y2-y)) + ...
	bsxfun(@times, fQ21, (x-x1).*(y2-y)) + ...
	bsxfun(@times, fQ12, (x2-x).*(y-y1)) + ...
	bsxfun(@times, fQ22, (x-x1).*(y-y1));

    %To make it general, uncomment this line and modify calc of x2 y2
    %b = bsxfun(@rdivide, b, ( (x2-x1).*(y2-y1) ));

end
