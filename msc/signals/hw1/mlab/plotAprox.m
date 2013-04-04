function [ output_args ] = plotAprox(tk,f,fs)
%PLOTAPROX Summary of this function goes here
%   Detailed explanation goes here

x = linspace(tk(1),-tk(1),int32(20));
plot(x,f(x),'r')
hold on
stairs(tk,fs,'b')

end

