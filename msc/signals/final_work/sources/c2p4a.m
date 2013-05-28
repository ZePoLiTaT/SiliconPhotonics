Sa=@(x) sin(x)./x;

LIM = 100*pi;
w = -1.5*LIM:LIM/1000:1.5*LIM;
Xw = 2 * ( Sa(2*(w-200)) + Sa(2*(w+200)) );

plot(w,abs(real(Xw)));
grid on;
xlim([-round(1.5*LIM) round(1.5*LIM)]);
xlabel('$\omega$','interpreter','latex');
title('$|X(\omega)|$','interpreter','latex');
