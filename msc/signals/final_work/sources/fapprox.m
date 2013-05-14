function [x,t] = fapprox(N,T)    
    t = -1.5*T:T/1000:1.5*T;
    w0 = 2*pi/T;    
    X0 = 0;
    x = X0*ones(1,length(t)); % dc component
    for n=1:N,
     Xn = -4*j/n/pi*sin(pi*n/6)*sin(n*pi/2)*exp(-j*n*pi/3);
     X_n = conj(Xn);
     x = x + Xn*exp(j*n*w0*t) + X_n*exp(-j*n*w0*t);
    end
end