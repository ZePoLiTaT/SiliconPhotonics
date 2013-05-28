fe = [-50*pi 0 50*pi];
Ae = [pi 4*pi pi];

figure(1); 
stem(fe,Ae);
grid on;
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('|X_\omega|', 'fontweight', 'bold');

