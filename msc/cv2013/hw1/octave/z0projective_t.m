%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
% HW1 Solution
% Part 1: ...
% Part 2: ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
graphics_toolkit fltk;
clear; clc; close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Load transformed image >>projected<<
[ztransf, imgtransf, W, H] = zimread('../imgs/tiles.png');
%[ztransf, imgtransf, W, H] = zimread('../imgs/test_2.jpg');

imshow(imgtransf);
hold on;

%Read 4 pts from the corners
[xp,yp] = ginput(4);
scatter(xp,yp,5,'r');

%TODO: Ask how to get these values o.O
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
imgtransf2 = ones(H,W,3);
for i=1:size(imgtransf2)(1)
    for j=1:size(imgtransf2)(2)
	tx = Hmat*[j i 1]';
	tx = int32(tx./tx(3));
	if(tx(1)>0 && tx(1)<= size(imgtransf2)(2) &&
	   tx(2)>0 && tx(2)<= size(imgtransf2)(1))
		%imgtransf2(tx(2),tx(1),:)=imgtransf(i,j,:);
		imgtransf2(i,j,:)=imgtransf(tx(2),tx(1),:);
	end
    end
end

figure(2);
imshow(uint8(imgtransf2));

