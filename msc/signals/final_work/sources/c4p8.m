f = @(t) 2*exp(-2*t);
Fw = @(w) 2./(2+j*w);

%%%%%%%%%% ANALYTICAL TRANSFORM %%%%%%%%%%%
%Plot the analytical expression of the Fourier Transform
LIM = 200;
w = [0:0.1:LIM];
exactFw = Fw(w);

%%%%%%%%%%%%%%%%% FFT %%%%%%%%%%%%%%%%%%%%%

N = 1000;
T = pi/100;

t = [ 0: T : T*(N -1) ]
k = [ 0: N-1]
approxW = 2*pi*k/N
approxFw  = (1 - exp(-j*approxW))./(j*approxW/T) .* fft(f(t));

subplot(211), plot(w,abs(exactFw), 'b')
grid on
title('$\|X(\omega)\|$','interpreter','latex');

subplot(212), plot(approxW/T, abs(approxFw),'r');
grid on;
title('$FFT Approximation$','interpreter','latex');
ylabel(['N=', num2str(N), ' T=', num2str(T)]);