n=1:15;
Xn=-4*j./n/pi.*sin(pi*n/6).*sin(n*pi/2).*exp(-j*n*pi/3);
n=-15:-1;
X_n=-4*j./n/pi.*sin(pi*n/6).*sin(n*pi/2).*exp(-j*n*pi/3);
Xn=[X_n 0 Xn];
n=-15:15;
subplot(211),stem(n,abs(Xn));
title('|X_n|')
subplot(212),stem(n,angle(Xn))
title('angle(X_n) in rad')