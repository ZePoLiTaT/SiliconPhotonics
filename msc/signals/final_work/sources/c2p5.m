LIM = 10*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 0.5 * ( 1./(3+(w-10)*j) + 1./(3+(w+10)*j));

plot(w,abs(real(Xw)));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$\|X(\omega)\|$','interpreter','latex');