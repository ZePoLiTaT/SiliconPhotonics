%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% HW1 Solution
% Part 1: Remove projective distortion from an image

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear; clc; close all;

[iVecImg, iImg, iW, iH] = zimread('../imgs/scan.jpg');
xp=[32  354 528 138];
yp=[204 138 568 677];
x =[100 460 460 100];
y =[150 150 670 670];

imshow(iImg);
hold on;

%graphics_toolkit fltk;
%[xp,yp] = ginput(4);
%x=[0;iW;iW;0];
%y=[0;0;iH;iH];
scatter(xp,yp,5,'r');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find transformation matrix from 2 sets of 4 points in the
%original and transformed image
Hmat = zfindH(x,y,xp,yp);
Hinv = inv(Hmat);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Finds the corresponding original no transformed points (x,y) 
[fSize, fBounds] = transformCorner(Hinv, [1,iW; 1,iH]);
iPos = transformImg(Hmat, fBounds);
fH = fSize(2); fW = fSize(1);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%find valid position of the indexes refering to the original image
iX = find(iPos(:,1)>=1 & iPos(:,2)>=1 & iPos(:,1)<(iW-1) & iPos(:,2)<(iH-1));

%find the the indexes (ix) of fPos from the unwrapped vector iPos
fPos = [1:size(iPos,1)]';

%only store the valid iPos and fPos
iPos = iPos(iX,:);
fPos = fPos(iX,:);

%create original image of size fWxfH
fVecImg = zeros(fH*fW, 3);

%assign the bilinear interpolation from the rgb channel of the transformed image (x',y') 
%into the equivalent original image (x,y)
fVecImg(fPos,:) = bilinear(iVecImg, iPos, iH);

%Transform vector to matrix image
fImg = zimasmat(fVecImg, fW, fH);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(2);
imshow(fImg);
%imwrite(iImg, 'out1.jpg');
imwrite(fImg, 'out2.jpg');
