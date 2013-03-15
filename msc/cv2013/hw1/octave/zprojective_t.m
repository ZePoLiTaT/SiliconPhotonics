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
%[ztransf, imgtransf, W, H] = zimread('../imgs/building.png');
[ztransf, imgtransf, W, H] = zimread('../imgs/tiles.png');

imshow(imgtransf);
hold on;

%Read 4 pts from the corners
[x,y] = ginput(4);
scatter(x,y,5,'r');

%TODO: Ask how to get these values o.O
%xp=[50;130;130;50];
%yp=[50;50;130;130];
xp=[0;W;W;0];
yp=[0;0;H;H];



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find transformation matrix from 2 sets of 4 points 
Hmat = zfindH(x,y,xp,yp);
Hinv = pinv(Hmat);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Finds the corresponding original no transformed points (x,y) 

ix1dorig = zfindX(Hinv,W,H);

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
