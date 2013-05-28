w = [-15*pi 0 15*pi];
Xw = [pi 2*pi pi];

ws = 20*pi;
w = [w-ws w w+ws];
Xw = [Xw Xw Xw];

stem(w,Xw);
grid on;
%xlim([-10*pi-2 10*pi+2]);  %passband filter
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('|x_\omega|', 'fontweight', 'bold');
