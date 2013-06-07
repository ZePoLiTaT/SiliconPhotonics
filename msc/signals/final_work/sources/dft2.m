%
% Discrete fourier transform <my version>
%
function xk = dft2(xn, N)
    
    %If N is given and is greater than xn, then do 0 padding
    if N > length(xn)
        xn = [xn zeros(1, N - length(xn))];
    else
        N = length(xn);
    end
    
    n = [0:N-1];
    k = [0:N-1];
    
    %create a matrix of nxk with all the combinations 
    expnk = exp( (-2*pi*j/N) * (n'*k) );
    
    xk = xn*expnk;
end