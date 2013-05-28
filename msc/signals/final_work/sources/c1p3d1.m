fe =  [-2 -1 0  1 2];
Ade = [ 1  0 0 -1 -1];
Ae =  [ 1 -1 0 -1 1];

figure(1); 
subplot(1,2,1), stairs(fe,Ade);
grid on;
axis([-2 2 -2 2]);
xlabel('t');
ylabel('$\dot{x}$','interpreter','latex');
title('$\dot{x}$','interpreter','latex', 'fontweight', 'bold');

subplot(1,2,2), stem(fe,Ae);
grid on;
axis([-2 2 -2 2]);
xlabel('t');
ylabel('$\ddot{x}$','interpreter','latex');
title('$\ddot{x}$','interpreter','latex', 'fontweight', 'bold');

