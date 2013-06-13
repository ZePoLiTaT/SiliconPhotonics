function [beta, in_count, in_idx, cont] = ransac(x,y, n)
% RANSAC Ejecuta el algoritmo de ransac para encontrar los parametros
% que mejor describan el modelo lineal

    ntot = length(y);

    %% Parametros de RANSAC. 
    % Se desea que el algoritmo no continúe si se está seguro 
    % en un 99% que la siguiente iteración no contiene outliers
    p = 0.99;
    M = (1-0.25)*ntot;

    N = Inf;
    cont = 0;
    ep = 0.5;

    max_si = -1;
    max_si_idx = [];

    %% Ciclo principal
    while cont < N
        %% 1. Seleccionar muestra aleatoria y obtener modelo
        [xsa, ysa] = random_sample(x,y, n);
        beta = get_model(xsa, ysa);
        %% 2. Calcular el soporte mediante la cantidad de inliers
        [si, si_idx] = calc_si(x, beta, y);

        %Almacenar mejor concenso
        if (si > max_si)
            max_si = si;
            max_si_idx = si_idx;
        end

        %% 3. Verificar si se acepta o no el modelo       
        if (si >= M)
            break 
        end

        %% 4. Recalcular la probabilidad de inliers y el num de iteraciones
        eptmp = 1 - si/ntot;
        if(eptmp < ep)
            ep = eptmp;
            N = log(1-p)/log(1-(1-ep)^n)
        end        
        
        cont = cont + 1;
    end

    %% 5. Reestimar el modelo a partir de todos los inliers
    beta = get_model(x(max_si_idx,:), y(max_si_idx));
    in_count = max_si;
    in_idx = max_si_idx;
end