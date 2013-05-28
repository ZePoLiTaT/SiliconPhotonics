LIM = 2.5*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 0.5* (1./(0.25 + (w-1)*j) + 1./(0.25 + (w+1)*j));
Xw = abs(real(Xw));

ws = 8;
w = [w-ws w w+ws];
Xw = [Xw Xw Xw];

plot(w,Xw);
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
ylim([-0.5 2.5]);
xlabel('$\omega$','interpreter','latex');
title('$|X_s(\omega)|$','interpreter','latex');
