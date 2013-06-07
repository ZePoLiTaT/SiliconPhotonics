%function handlers declaraton
dtft_pa = @(w) 0.5 * exp(-j * 1.5 * w) .* ( cos(w*1.5) + cos(w*0.5) );
dtft_pb = @(w) 2 * exp(-j*w) .* (cos(w)-1);
dtft_pc = @(w) 2 ./(1 - 3/4*exp(-j*w) );


N = 32;
k = [0:N-1];
w = [0:0.001:2*pi];

figure(1);
%%%%%%%%%%%%%%%%%%%%% POINT (a) %%%%%%%%%%%%%%%%%%%%%%%
xn_pa = [1/4 1/4 1/4 1/4];

xk_pa = dft2(xn_pa, N);
xw_pa = dtft_pa(w);

%Plot blue continuous frecuency vs red dots discrete frec
subplot(2,1,1);
plot(2*pi*k/N, abs(xk_pa), 'ro', w, abs(xw_pa),'b');

%%%%%%%%%%%%%%%%%%%%% POINT (b) %%%%%%%%%%%%%%%%%%%%%%%
xn_pb = [1 -2 1];

xk_pb = dft2(xn_pb, N);
xw_pb = dtft_pb(w);

%Plot blue continuous frecuency vs red dots discrete frec
subplot(2,1,2);
plot(2*pi*k/N, abs(xk_pb), 'ro', w, abs(xw_pb),'b');

figure(2);
%%%%%%%%%%%%%%%%%%%%% POINT (c) %%%%%%%%%%%%%%%%%%%%%%%
N = [5 10 20]
for i = 1:length(N)
    k = [0 : N(i)-1];
    xn_pc = 2*(3/4).^k;

    xk_pc = dft2(xn_pc, N(i));
    xw_pc = dtft_pc(w);

    %Plot blue continuous frecuency vs red dots discrete frec
    subplot(3,1,i);
    plot(2*pi*k/N(i), abs(xk_pc), 'ro', w, abs(xw_pc),'b');
end