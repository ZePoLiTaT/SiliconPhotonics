function beta = get_model(x,y)
% GET_MODEL Calcula los parametros del modelo mediate Linear Least Squares Method
% minimizando X*beta= y
%   beta = GET_MODEL(x,y) Encuentra beta mediante LLS

    beta = (x'*x)\x'*y;
    
end