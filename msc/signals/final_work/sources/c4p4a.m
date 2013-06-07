fHw = @(w) cos(w/2) .* exp(-j * w);

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

fe = [-2*pi/3 -pi/4 0 pi/4 2*pi/3];
Ae = [pi 2*pi 4*pi 2*pi pi];
pe = [pi/2 0 0 0 -pi/2];

subplot(3,2,3), stem(fe,Ae);
grid on;
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('$|X(\omega)|$','interpreter','latex');

subplot(3,2,4), stem(fe,pe);
grid on;
xlabel('Frec [rad/s]');
ylabel('Phase');
title('$\angle X(\omega)$','interpreter','latex');


fe = [-2*pi/3 -pi/4 0 pi/4 2*pi/3];
Ae = [pi*abs(fHw(-2*pi/3))  2*pi*abs(fHw(-pi/4))  4*pi  2*pi*abs(fHw(pi/4))  pi*abs(fHw(2*pi/3))];
pe = [pi/2+2*pi/3 pi/4 0 -pi/4 -pi/2-2*pi/3];

subplot(3,2,5), stem(fe,Ae);
grid on;
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('$|Y(\omega)|$','interpreter','latex');

subplot(3,2,6), stem(fe,pe);
grid on;
xlabel('Frec [rad/s]');
ylabel('Phase');
title('$\angle Y(\omega)$','interpreter','latex');