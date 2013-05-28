fe = [0 3 4];
Ae = [1 2 -3];

figure(1); 
stem(fe,Ae);
grid on;
xlim([-1 5]); ylim([-4 3]);
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('$\dot{x}$','interpreter','latex');