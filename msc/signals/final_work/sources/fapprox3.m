function [x,t] = fapprox3(N,T)    
    t = -1.5*T:T/1000:1.5*T;
    w0 = 2*pi/T;
    X0 = 3/4;
    
    n_p = [1:N];
    n_n = [-N:-1];
    Xn = (2 - (-1).^n_p - (-j).^n_p) ./ (2*j*pi.*n_p);
    X_n = (2 - (-1).^n_n - (-j).^n_n) ./ (2*j*pi.*n_n);    
    
    Xn = [X_n X0 Xn]; 
    n = [n_n 0 n_p];
    
    x = Xn*exp(j*w0*n'*t);
    x = real(x);
end