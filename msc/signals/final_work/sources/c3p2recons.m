u = @(x)(x>=0);

i=1;
for T=[0.05, pi/4, pi/2, 2*pi/3] 
    t=[0:T:15];
    x=exp(-t/4).*cos(t).*u(t);
    
    subplot(4,1,i), plot(t,x);
    grid on;
    xlabel('t');
    title(['Reconstructed x(t) with T=' num2str(T)]);
    i = i + 1;
end