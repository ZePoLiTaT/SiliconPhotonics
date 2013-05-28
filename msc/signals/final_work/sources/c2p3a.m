LIM = 2*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 4*j./w .* exp(-2*j*w) .* (2*exp(-2*j*w) -  sin(2*w)./(2*w));

subplot(1,2,1), plot(w,abs(real(Xw)));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$|X(\omega)|$','interpreter','latex');

subplot(1,2,2), plot(w,angle(Xw));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$\angle X(\omega)$','interpreter','latex');
