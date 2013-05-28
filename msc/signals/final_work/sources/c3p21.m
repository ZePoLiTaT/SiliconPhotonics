LIM = 0.5*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 0.5* (1./(0.25 + (w-1)*j) + 1./(0.25 + (w+1)*j));

plot(w,abs(real(Xw)));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$|X(\omega)|$','interpreter','latex');