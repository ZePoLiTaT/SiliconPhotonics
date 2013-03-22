%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% HW1 Solution
% Part 1: ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
graphics_toolkit fltk;
clear; clc; close all;
% {
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Load transformed image >>projected<<
%[iVecImg, iImg, iW, iH] = zimread('../imgs/building.png');
[iVecImg, iImg, iW, iH] = zimread('../imgs/QR2.jpg');
%[iVecImg, iImg, iW, iH] = zimread('../imgs/tiles.png');
%[iVecImg, iImg, iW, iH] = zimread('../imgs/test_1.jpg');


imshow(iImg);
hold on;


%Read 4 pts from the corners
[xp,yp] = ginput(4);
scatter(xp,yp,5,'r');

x=[50;130;130;50];
y=[50;50;130;130];
%x=[0;W;W;0];
%y=[0;0;H;H];
% }
%{ 
[iVecImg, iImg, iW, iH] = zimread('../imgs/uno.jpg');
x=[19 605 605 19];
y=[28 28 466 466];
xp=[142 491 504 146];
yp=[2 98 379 470];
%}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find transformation matrix from 2 sets of 4 points in the
%original and transformed image
Hmat = zfindH(x,y,xp,yp);
Hinv = inv(Hmat);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Finds the corresponding original no transformed points (x,y) 
[fSize, fBounds] = transformCorner(Hinv, [iW iH]);
iPos = transformX(Hmat,[iW iH], fBounds);
fH = fSize(2); fW = fSize(1);

%{
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%find valid position of the index
validpos = find(ix1dorig>0);

%create original image
zorig = ones(fSize(1) * fSize(2), 3);

%assign rgb channel of the transformed image (x',y') into the equivalent original image (x,y)
zorig(validpos,:) = iVecImg(ix1dorig(validpos),:);
%zorig = iVecImg(ix1dorig,:);

%transform vector to matrix image
imgorig = zimasmat(zorig, fSize(1), fSize(2));

figure(2);
imshow(imgorig);
%}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% {
%find valid position of the indexes refering to the original image
[fX] = find(iPos(:,1)>=1 & iPos(:,2)>=1 & iPos(:,1)<(iW-1) & iPos(:,2)<(iH-1));

%find the x,y coordinates from the indexes (ix) of the unwrapped vector iPos
% y = f = (ix-1)%(H-1) + 1
% x = c = ceil(ix/H)
fPos = [1:size(iPos,1)]';
%fPos = [ceil(fPos/fH)+1 , mod(fPos-1,fH-1)+1 ]; 

%only store the valid iPos and fPos
iPos = iPos(fX,:);
fPos = fPos(fX,:);

%create original image of size fWxfH
fVecImg = zeros(fH*fW, 3);

%assign the bilinear interpolation from the rgb channel of the transformed image (x',y') into the equivalent original image (x,y)
fVecImg(fPos,:) = bilinear(iVecImg, iPos, iH);

%Transform vector to matrix image
fImg = zimasmat(fVecImg, fW, fH);

figure(3);
imshow(fImg);
%imwrite(fImg, 'out.jpg');
% } 
