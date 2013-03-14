img = imread('imgs/tiles.png');

graphics_toolkit fltk

imshow(img);
hold on;

%Read 4 pts from the corners
[x,y] = ginput(4);
scatter(x,y,5,'r');

%TODO: Ask how to get these values o.O
xp=[50;130;130;50];
yp=[50;50;130;130];

A = zeros(8,8);
B = zeros(8,1);

for i=1:4
    A(2*i-1,:)=[x(i) y(i) 1 0 0 0 -xp(i)*x(i) -xp(i)*y(i)];
    A(2*i,:) = [0 0 0 x(i) y(i) 1 -yp(i)*x(i) -yp(i)*y(i)];

    B(2*i-1) = xp(i);
    B(2*i) = yp(i);
end

H=A\B;
H=reshape([H;1],3,3)';
Hp=pinv(H);


img2 = img;
for i=1:size(img2)(1)
    for j=1:size(img2)(2)
        img2(i,j,:)=img(1,1,:);             
    end 
end 

for i=1:size(img)(1)
    for j=1:size(img)(2)
	tx = Hp*[i j 1]';
	tx = uint8(tx./tx(3));
	if(tx(1)>0 && tx(1)< size(img2)(1) &&
	   tx(2)>0 && tx(2)< size(img2)(2))
		img2(tx(1),tx(2),:)=img(i,j,:);
	end
    end
end

figure(2);
imshow(img2);
