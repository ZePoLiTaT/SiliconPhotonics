[iVecImg, iImg, iW, iH] = zimread('../imgs/scan.jpg');
xp=[32  354 528 138];
yp=[204 138 568 677];
x =[100 460 460 100];
y =[150 150 670 670];

imshow(iImg);
hold on;
scatter(xp,yp,5,'r');


