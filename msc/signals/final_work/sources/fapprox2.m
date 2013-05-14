function [x,t] = fapprox2(N,T)      
    w0 = 2*pi/T;
    t = -1.5*T:T/1000:1.5*T;
    c0 = 1/5;
    x = c0*ones(1,length(t)); % dc component
    for n=1:N,
     cn = cos(pi/2*n*w0)/5/(1-(n*w0)^2);
     c_n = cn;
     x = x + cn*exp(j*n*w0*t) + c_n*exp(-j*n*w0*t);
    end
    plot(t,x)
    title([' N = ',num2str(N)])
end