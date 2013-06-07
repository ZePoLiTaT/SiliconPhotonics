f = @(t) 4*exp(-t);
Fw = @(w) 4./(1+j*w);

%%%%%%%%%% ANALYTICAL TRANSFORM %%%%%%%%%%%
%Plot the analytical expression of the Fourier Transform
LIM = 20;
w = [0:0.1:LIM];
exactFw = Fw(w);

%%%%%%%%%%%%%%%%% FFT %%%%%%%%%%%%%%%%%%%%%

N = [10 20 20 100];
T = [1 1 0.5 0.1];

for i=1:length(N)
   t = [ 0: T(i) : T(i)*(N(i) -1) ]
   k = [ 0: N(i)-1]
   approxW = 2*pi*k/N(i)
   approxFw  = (1 - exp(-j*approxW))./(j*approxW/T(i)) .* fft(f(t));
   subplot(2,2,i), plot(w,abs(exactFw), 'b',approxW/T(i), abs(approxFw),'ro');
   ylabel(['N=', num2str(N(i))]);
   grid on;
end

title('$|X(\omega)|$','interpreter','latex');