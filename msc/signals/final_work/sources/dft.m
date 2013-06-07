%
% Discrete Fourier Transform
%
function Xk = dft(x)

[N, M] = size(x);
% makes sure that x is a column vector
if M ~= 1
N = M;
x = x';
end
Xk = zeros (N, 1);
n = 0:N-1;
for k = 0:N-1
    Xk(k + 1) = exp(-j*2*pi*k*n/N)*x;
end