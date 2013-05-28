w = [-60*pi -20*pi 0 20*pi 60*pi];
Xw = [pi pi 2*pi pi pi];

figure(1); 
stem(w,Xw);
grid on;
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('|X_\omega|', 'fontweight', 'bold');

 