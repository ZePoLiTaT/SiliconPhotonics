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

[iVecImg, iImg, iW, iH] = zimread('../imgs/hw2pdf.jpg');
iSize=[iW iH];
imshow(iImg);
hold on;

% Points that are in two sets parallel lines
% when the perspective transformation in removed
x =[167   217   194   244];
y =[186   222   169   202];

%x = [155 247 245 336];
%y = [266 344 201 262];

%graphics_toolkit fltk;
%[x,y] = ginput(4);
scatter(x,y,5,'c');

%Find the H that removes the perspective with the vanishing line
Hvl = eye(3);
Hvl(3,:) = findVL([x(1);y(1);1],[x(2);y(2);1],[x(3);y(3);1],[x(4);y(4);1]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Transform the corners to get the new size of the image
[fSize, fBounds] = transformCorner(Hvl, [1,iSize(1); 1,iSize(2)]);

%sc = iW/fSize(1)
%fSize = round(fSize*sc);
%fSize = round(fSize);
%fSize = [iW iH];

%Hvl2 = Hvl*[sc 0 0;0 sc 0;0 0 1]
%Hvl2 = [sc 0 0;0 sc 0;Hvl(3,:)]
Hinv = inv(Hvl);

iPos = transformImg(Hinv, [1 1 fSize]);
fImg = createTransfImg(fSize, iSize, iPos, iVecImg);
%{
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% find valid position of the indexes refering to the original image
iX = find(iPos(:,1)>=1 & iPos(:,2)>=1 & iPos(:,1)<(iW-1) & iPos(:,2)<(iH-1));
% find the the indexes (ix) of fPos from the unwrapped vector iPos
fPos = [1:size(iPos,1)]';

% create original image of size fWxfH
fVecImg = zeros(fSize(2)*fSize(1), 3);
% assign the bilinear interpolation from the rgb channel of the transformed image (x',y') 
% into the equivalent original image (x,y)
fVecImg(fPos(iX,:),:) = bilinear(iVecImg, iPos(iX,:), iH);
% Transform vector to matrix image
fImg = zimasmat(fVecImg, fSize(1), fSize(2));
%}
figure(2);
imshow(fImg);
hold on;
%imwrite(iImg, 'out1.jpg');
%imwrite(fImg, 'out2.jpg');


%[x,y] = ginput(6);
x = [261.297    19.648    47.696   111.653    72.354   103.177];
y = [ 97.647    41.809    58.572    39.382   124.121    91.983];
p = [x; y; ones(size(x))];

l = cross(p(:,1) , p(:,2));
m = cross(p(:,2) , p(:,3));
n = cross(p(:,4) , p(:,5));
p = cross(p(:,5) , p(:,6));

A = [l(1)*m(1) l(1)*m(2)+l(2)*m(1) l(2)*m(2);
     n(1)*p(1) n(1)*p(2)+n(2)*p(1) n(2)*n(2)];
[U D V] = svd(A);
s = V(end,:);
S=[s(1) s(2);s(2) s(3)];
A=chol(S, 'lower');

Ha = [A [0;0];[0 0] 1];

