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
%[ztransf, imgtransf, W, H] = zimread('../imgs/test_1.jpg');

%Create original image
zorig = ones(size(ztransf));

imshow(imgtransf);
hold on;

%Read 4 pts from the corners
[xp,yp] = ginput(4);
scatter(xp,yp,5,'r');

x=[50;130;130;50];
y=[50;50;130;130];
%x=[0;W;W;0];
%y=[0;0;H;H];


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find transformation matrix from 2 sets of 4 points in the
%original and transformed image
Hmat = zfindH(x,y,xp,yp);
Hinv = pinv(Hmat);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Finds the corresponding original no transformed points (x,y) 
ix1dorig = zfindX(Hmat,[W],[H]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find valid position of the index
validpos = find(ix1dorig>0 & ix1dorig<=(W*H));

%Assign RGB channel of the transformed image (x',y') into the equivalent original image (x,y)
zorig(validpos,:) = ztransf(ix1dorig(validpos),:);

%Transform vector to matrix image
imgorig = zimasmat(zorig, W, H);

figure(2);
imshow(imgorig);


