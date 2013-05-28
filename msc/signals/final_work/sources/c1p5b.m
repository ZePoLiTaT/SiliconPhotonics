figure(1);

T=6;
N = [5,30];

for i=1:length(N)
    [x,t] = fapprox5(N(i),T);    
    subplot(length(N),1,i), plot(t,x);    
    grid on;
    title([' N = ',num2str(N(i))])
end
