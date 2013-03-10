%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Plots a conic given 5 points in a plane   %
%					    %
% ~.ZePoLiTaT.~				    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

graphics_toolkit fltk

XMAX = 400;
YMAX = 400;

img = zeros(XMAX, YMAX);
imshow(img);
hold on;

%read 5 points from figure
[x,y] = ginput(5);
scatter(x,y,5,'r');

%Define param matrix
A=[x.^2 x.*y y.^2 x y];
c = A \ (ones(5,1) * -1);

C=[ c(1) c(2)/2 c(4)/2;
    c(2)/2 c(3) c(5)/2;
    c(4)/2 c(5)/2 1]

for px = 1:XMAX
    for py = 1:YMAX
	img(px,py) = [py px 1]*C*[py px 1]';
    end
end

pic=(img>0)*0.3+((abs(img)<1e-3)*0.6);
imshow(pic);
scatter(x,y,5,'r');
