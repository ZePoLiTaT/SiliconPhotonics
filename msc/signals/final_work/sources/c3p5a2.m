w = [-60*pi -20*pi 0 20*pi 60*pi];
Xw = [pi pi 2*pi pi pi];

ws = 200*pi;
w = [w-ws w w+ws];
Xw = [Xw Xw Xw];

figure(1); 
stem(w,Xw);
grid on;
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('|X_\omega|', 'fontweight', 'bold');

 