%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#
% HW1 Solution
% Part 2: ...

% Author: Tatiana L. G. <tatiana@sirius.utp.edu.co>
% Created: 
% Version: 0.0.1 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#

clear; clc; close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Load transformed image >>projected<<
[vPersp, iPersp, W, H] = zimread('../imgs/room.jpg');
[vPlanar, iPlanar, WPlanar, HPlanar] = zimread('../imgs/domokun.jpg');


%The original corrdinates are the boundaries of the planar img
x =[1;WPlanar;WPlanar;1];
y =[1;1;HPlanar;HPlanar];
xp=[722  897  898  726];
yp=[253  148  503  467];

imshow(iPlanar);
figure(2);
imshow(iPersp);
hold on;
scatter(xp,yp,5,'r');


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find transformation matrix from 2 sets of 4 points in the
%original and transformed image
Hmat = zfindH(x,y,xp,yp);
Hinv = pinv(Hmat);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Finds the corresponding original no transformed points (x,y) 
ix1dorig = zfindX(Hmat,[WPlanar W],[HPlanar H]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Find valid position of the index
validpos = find(ix1dorig>0 & ix1dorig<=(W*H));

%Assign RGB channel of the transformed image (x',y') into the equivalent original image (x,y)
vPersp(ix1dorig(validpos),:) = vPlanar(validpos,:);

%Transform vector to matrix image
iPerspMod = zimasmat(vPersp, W, H);

figure(3);
imshow(iPerspMod);

imwrite(iPerspMod, 'p2fImg.jpg');
