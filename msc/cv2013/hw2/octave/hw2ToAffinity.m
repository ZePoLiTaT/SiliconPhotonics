%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% HW2 Solution
% Part 1: Remove perspective from an image by analizing
% structures that appear inside it

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: Apr 1 2013
% Version: 0.0.1 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear; clc; close all;
fname = '../imgs/hw2pdf';
[iVecImg, iImg, iW, iH] = zimread(strcat(fname,'.jpg'));
iSize=[iW iH];
imshow(iImg);
hold on;

% Points that are in two sets parallel lines
% when the perspective transformation in removed
x =[30.335   157.271   181.257   313.815];
y =[154.534   266.976    74.576   148.912];
%graphics_toolkit fltk;
%[x,y] = ginput(4); x=x'; y=y';
p = [x; y; ones(size(x))];

%Find the H that removes the perspective with the vanishing line
[VL,l] = findVL(p(:,1), p(:,2), p(:,3), p(:,4));

scatter(x,y,5,'c');
drawLine(linspace(1,iW,iW), l);

Hp = [1 0 0;0 1 0;VL'];
HpInv = inv(Hp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Transform the corners to get the new size of the image
fSize = transformCorner(Hp, [1,iSize(1); 1,iSize(2)]);
iPos = transformImg(HpInv, [1 1 fSize]);
[fImg, fVecImg] = createTransfImg(fSize, iSize, iPos, iVecImg);

figure(2); imshow(fImg); hold on;

lo = HpInv'*l;
drawLine(linspace(1,fSize(1),fSize(1)), lo);

imwrite(fImg, strcat(strcat(fname,'-toAffinity'),'.jpg'));
