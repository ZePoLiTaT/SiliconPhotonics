fe = [-1 0 1 2 3 4];
Ae = [0 2 -1 -1 0 2];

figure(1); 
stem(fe,Ae);
grid on;
xlabel('t');
ylabel('$\dot{x}$','interpreter','latex');
title('$\dot{x}$','interpreter','latex', 'fontweight', 'bold');

