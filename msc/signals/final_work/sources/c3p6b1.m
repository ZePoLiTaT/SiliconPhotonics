LIM = 1.5*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 1./(j*w + 1);

subplot(2,1,1), plot(w,abs(real(Xw)));
grid on;
xlabel('$\omega$','interpreter','latex');
title('$|X(\omega)|$','interpreter','latex');

%FWHM
hold on
stem(sqrt(3),1./(j*sqrt(3) + 1))

ws = 2*pi;
w = [w-ws w w+ws];
Xw = [Xw Xw Xw];

subplot(2,1,2), plot(w,abs(real(Xw)));
grid on;
%xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$|X_s(\omega)|$','interpreter','latex');

%FWHM
hold on
stem(sqrt(3),1./(j*sqrt(3) + 1))

