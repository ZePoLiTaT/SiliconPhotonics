fHw = @(w) 2*j .* exp(-j * w / 2) .* sin(w/2) ;

LIM = 0.7*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Hw = fHw(w);

subplot(3,2,1), plot(w,abs(Hw));
grid on;
xlim([-round(1.5*LIM) abs(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$|H(\omega)|$','interpreter','latex');

subplot(3,2,2), plot(w,angle(Hw));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$\angle H(\omega)$','interpreter','latex');