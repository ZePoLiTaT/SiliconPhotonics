clf;
img2 = img;
for i=1:size(img)(1)
    for j=1:size(img)(2)
	tx = Hp*[i j 1]';
	tx = uint8(tx./tx(3));
	if(tx(1)>0 && tx(1)< size(img2)(1) &&
	   tx(2)>0 && tx(2)< size(img2)(2))
		img2(tx(1),tx(2),:)=img(i,j,:);
%		img2(tx(1),tx(2),2)=img(i,j,2);
%		img2(tx(1),tx(2),3)=img(i,j,3);
	end
    end
end

%figure(2);
imshow(img2);
