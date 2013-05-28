w = [-15*pi 0 15*pi];
Xw = [pi 2*pi pi];

figure(1); 
stem(w,Xw);
grid on;
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('|X_\omega|', 'fontweight', 'bold');
