function [si, si_idx] = calc_si(x, beta, y)
% CALC_SI Obtiene el numero de inliers del modelo mediante su distancia
% algebraica al modelo
%   [si, si_idx] = CALC_SI(x, beta, y) El modelo dado por |x*beta - yr| < 3*sigma

    dalg = abs(x*beta - y);
    sigma = 0.05; %std(dalg);
    
    si_idx = dalg < 3*sigma;
    si = sum(si_idx);
end