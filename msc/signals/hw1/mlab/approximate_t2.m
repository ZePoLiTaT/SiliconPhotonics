function [ tk, fs ] = approximate_t2( j, t )
%APPROXIMATE Aproximación de la función t^2 mediante steps

j2=2^j;
k=[j2*(-t) : j2*(t)-1]
fs = 1/(j2^2) * (k.^2 + k + 1/3)
tk = k/j2
%plot(tk,tk.^2,'r')
%hold on
%stairs(tk,fs,'b')

end

