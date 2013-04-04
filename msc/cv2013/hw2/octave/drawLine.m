%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% usage: drawLine(x, l)
%
% Plot a line or lines in homogeneous coord system
% ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: Apr 2 2013
% Version: 0.0.1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
function drawLine(x, l)

    tmp = bsxfun(@times, x', l(1,:)./l(2,:));
    ly = bsxfun(@minus, -l(3,:)./l(2,:), tmp);

    plot(x,ly);

    lvert = find(l(2,:)==0);
    if(~isempty(lvert))
        lvert = l(:,lvert);
	xvert = -lvert(3,:)./lvert(1,:);
        plot([xvert:xvert],[-7:0.1:7],'.g');
    end
end
