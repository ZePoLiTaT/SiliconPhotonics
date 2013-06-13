function [xsa, ysa] = random_sample(x,y, n)
% RANDOM_SAMPLE Obtiene una submuestra de n datos de los datos

    %Obtiene el valor de los n índices de forma aleatoria
    N = length(y);
    idx = randperm(N, n);
        
    %Extrae la submuestra con los índices de los datos
    xsa  = x(idx, :);
    ysa  = y(idx);
end