LIM = 0.7*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 2 * exp(-j*w) .* (cos(w)-1);

subplot(2,1,1), plot(w,abs(Xw));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$\|X(\omega)\|$','interpreter','latex');

subplot(2,1,2), plot(w,angle(Xw));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$\angle X(\omega)$','interpreter','latex');

