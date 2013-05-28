function [x,t] = fapprox4(N,T)    
    t = -1.5*T:T/1000:1.5*T;
    w0 = 2*pi/T;
    X0 = 3/5;
    
    n_p = [1:N];
    n_n = [-N:-1];
    Xn = (cos(n_p * w0) - cos(2*n_p * w0) ) * 2 ./ (5*w0^2*n_p.^2);
    X_n = (cos(n_n * w0) - cos(2*n_n * w0) ) * 2 ./ (5*w0^2*n_n.^2);
    
    
    Xn = [X_n X0 Xn]; 
    n = [n_n 0 n_p];
    
    x = Xn*exp(j*w0*n'*t);
    x = real(x);
end