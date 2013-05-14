t=[-5:0.01:5];
x=zeros(1,length(t));
zz=(abs(t)<=(pi/2));
x(zz) = cos(t(zz));

xd = diff(x); 
subplot(2,1,1), plot(t(2:end),xd);
title('$\dot{x}$','interpreter','latex')

%xdd = diff(xd); subplot(2,1,2), plot(t(3:end),xdd);
xdd = -x;
xdd(min(find(t>=pi/2 ))) = 1;
xdd(max(find(t<=-pi/2))) = 1;
subplot(2,1,2), plot(t,xdd);
title('$\ddot{x}$','interpreter','latex')