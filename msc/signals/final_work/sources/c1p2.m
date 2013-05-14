n=-10:10;
Xn=cos(pi/2*n*w0)/5./(1-(n*w0).^2);
subplot(121),stem(n,abs(Xn))
title('|X_n|')
subplot(122),stem(n,angle(Xn))
title('angle(X_n) in rad')