w = [-50*pi 0 50*pi];
Xw = [pi 4*pi pi];

ws = 80*pi;
w = [w-ws w w+ws];
Xw = [Xw Xw Xw];

stem(w,Xw);
grid on;
%xlim([-50*pi-2 50*pi+2]);
xlabel('$\omega$','interpreter','latex');
title('$|X_s(\omega)|$','interpreter','latex');
