fe = [-100 -50 0 50 100];
Ae = [6 2 2 2 6];
pe = [pi/3 -pi/2 0 pi/2 -pi/3];

figure(1); 
subplot(2,1,1), stem(fe,Ae);
grid on;
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('|X_n|', 'fontweight', 'bold');

subplot(2,1,2), stem(fe,pe);
grid on;
xlabel('Frec [rad/s]');
ylabel('Phase');
title('$\angle x$','interpreter','latex')
