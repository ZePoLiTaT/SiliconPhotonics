LIM = 4*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 1 - exp(-j*w*0.5) + exp(-j*w);
%Xw = sin(0.5*w)./(0.5*w) .* exp(-j*0.5*w);

subplot(1,2,1), plot(w,abs(Xw));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$|X(\omega)|$','interpreter','latex');

subplot(1,2,2), plot(w,angle(Xw));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$\angle X(\omega)$','interpreter','latex');