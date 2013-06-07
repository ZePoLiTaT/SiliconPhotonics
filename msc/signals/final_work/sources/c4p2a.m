dtft = @(w) 0.5 * exp(-j * 1.5 * w) .* ( cos(w*1.5) + cos(w*0.5) );

x = [1/4 1/4 1/4 1/4];
N = length(x);
k = [0:N-1];
w = 2*pi/N * k;

xdft = dft2(x, 0)
xdtft = dtft(w)

