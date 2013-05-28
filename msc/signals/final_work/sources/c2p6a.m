Sa=@(x) sin(x)./x;

LIM = -0.8*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 4 * Sa(w) .* exp(-3*j*w);

plot(w,abs(real(Xw)));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$|X(\omega)|$','interpreter','latex');
