fe = [0 2];
Ae = [2 -2];

figure(1); 
stem(fe,Ae);
grid on;
xlim([-1 3]); ylim([-3 3]);
xlabel('t');
ylabel('Amplitude');
title('$\dot{x}$','interpreter','latex')
