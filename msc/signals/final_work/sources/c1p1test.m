N=5;
[x,t]=fapprox(N,6);
subplot(2,1,1),plot(t,x),title([' N = ',num2str(N)])
N=50;
[x,t]=fapprox(N,6);
subplot(2,1,2),plot(t,x),title([' N = ',num2str(N)])