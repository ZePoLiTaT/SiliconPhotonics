dtft = @(w) 2 * exp(-j*w) .* (cos(w)-1);

x = [1 -2 1];
N = length(x);
k = [0:N-1];
w = 2*pi/N * k;

xdft = dft2(x, 0)
xdtft = dtft(w)
