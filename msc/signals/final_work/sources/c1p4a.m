figure(1);

T=4;
N = [3,10,40];

for i=1:3
    [x,t] = fapprox3(N(i),T);    
    subplot(3,1,i), plot(t,x);    
    title([' N = ',num2str(N(i))])
end
