%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% HW2 Solution
% Part 2: Remove affinity from an image by analizing
% structures that appear inside it

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: Apr 1 2013
% Version: 0.0.1 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear; clc; close all;
fname = '../imgs/hw2pdf-toAffinity';
[iVecImg, iImg, iW, iH] = zimread(strcat(fname,'.jpg'));
iSize=[iW iH];
imshow(iImg);
hold on;

% Points that belong to 2 sets of lines that are orthogonal
% when the perspective transformation in removed
x = [20    83   50    82   145  240];
y = [41    21    1   141    59   57];
p = [x; y; ones(size(x))];

% Find K matrix using two sets of orthogonal lines
[K,l] = findKMat(p);

%graphics_toolkit fltk;
%[x,y] = ginput(4);
scatter(x,y,5,'c');
drawLine(linspace(1,iW,iW), l);

Ha = [K [0;0];[0 0] 1];
HaInv = inv(Ha);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Transform the corners to get the new size of the image
fSize = transformCorner(HaInv, [1,iSize(1); 1,iSize(2)]);
iPos = transformImg(Ha, [1 1 fSize]);
[fImg, fVecImg] = createTransfImg(fSize, iSize, iPos, iVecImg);

figure(2); imshow(fImg); hold on;

lo = Ha'*l;
drawLine(linspace(1,fSize(1),fSize(1)), lo);

imwrite(fImg, strcat(strcat(fname,'-toSimilarity'),'.jpg'));
