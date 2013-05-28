function [x,t] = fapprox5(N,T)    
    t = -1.5*T:T/1000:1.5*T;
    w0 = 2*pi/T;
    X0 = 1;
    
    n_p = [1:N];
    n_n = [-N:-1];
    Xn = (1 + 2*(-1).^n_p - 3*exp(-4*j*n_p*w0) ) ./ (6*j*n_p*w0);
    X_n = (1 + 2*(-1).^n_n - 3*exp(-4*j*n_n*w0) ) ./ (6*j*n_n*w0);
    
    Xn = [X_n X0 Xn]; 
    n = [n_n 0 n_p];
    
    x = Xn*exp(j*w0*n'*t);
    x = real(x);
end