#######################################################
## HW1 Solution
## Part 1: ...
## Part 2: ...

## Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
## Created: 
## Version: 0.0.1 
#######################################################

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
graphics_toolkit fltk;
clear; clc; close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Load transformed image >>projected<<
[ztransf, imgtransf, W, H] = zimread('../imgs/tiles.png');

imshow(imgtransf);
hold on;

%Read 4 pts from the corners
[x,y] = ginput(4);
scatter(x,y,5,'r');

%TODO: Ask how to get these values o.O
xp=[50;130;130;50];
yp=[50;50;130;130];



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find transformation matrix from 2 sets of 4 points 
Hmat = zfindH(x,y,xp,yp);
Hinv = pinv(Hmat);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Generate matrix of points in transformed image:
%   [x1 x2 .. xn]
%   [y1 y2 .. yn]
%   [ 1  1 ..  1]
[xx,yy]=meshgrid([1:W],[1:H]);
ptransf=[xx(:)';yy(:)';ones(size(xx(:)'))];

%Find equivalent points in original (no transformed) image by
%using the inverse of the transformation matrix Hinv
porig = Hinv * ptransf;

%Normalize x1 and x2 by x3. Then make it integer because its an index
porig = int32(porig ./ porig(3,:));

%Find index in 1d vec of the 2d points of image:  i1d = (i2d.x-1)*W + i2d.y
ix1dorig = (porig(1,:) - 1)*H + porig(2,:);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Create new vector image from transformed indexes and transformed rgb channels
zorig = zmaptovec(ix1dorig, ztransf, W*H);
%Transform vector to matrix image
imgorig = zimasmat(zorig, W, H);

figure(2);
imshow(imgorig);



%{
imgtransf2 = imgtransf;
for i=1:size(imgtransf2)(1)
    for j=1:size(imgtransf2)(2)
        imgtransf2(i,j,:)=imgtransf(1,1,:);             
    end 
end 

for i=1:size(imgtransf)(1)
    for j=1:size(imgtransf)(2)
	tx = Hinv*[i j 1]';
	tx = uint8(tx./tx(3));
	if(tx(1)>0 && tx(1)< size(imgtransf2)(1) &&
	   tx(2)>0 && tx(2)< size(imgtransf2)(2))
		imgtransf2(tx(1),tx(2),:)=imgtransf(i,j,:);
	end
    end
end

figure(2);
imshow(imgtransf2);
%}
