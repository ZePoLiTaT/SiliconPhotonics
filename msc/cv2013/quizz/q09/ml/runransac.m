clear; clc; close all;
data = load('Quizz09data.csv','-ascii');

%% Cargar los valores del archivo generado por quizz09.m
% La primera fila son las Xs, la
% segunda los Y reales, y la tercera los YO observados con outliers.
ntot = size(data,2);
x = data(1,:)';
y= data(3,:)';
yr = data(2,:)';
x = [ones(ntot, 1) sin(x/10) sin(x/20)];

%% Calcular modelo tomando OUTLIERS
betaOUT = get_model(x,y);

%% Ejecutar RANSAC y obtener el modelo
[beta, in_count, in_idx] = ransac(x, y, 5);

%% Graficar el modelo obtenido vs el modelo real y los datos observados
plotf(data(1,:),y,x*beta,data(2,:), x*betaOUT)
plot(data(1,~in_idx), y(~in_idx), 'ro')


%% Simulacion de Monte Carlo
N = 1000;
rmse = 0;
cont= 0;
for i=1:N
    %% Ejecutar RANSAC y obtener el modelo
    [beta, in_count, in_idx, tcont] = ransac(x, y, 5);
    
    %% Calcular el RMSE
    er = yr - x*beta;
    rmse = rmse + (er'*er / ntot);
    cont = cont + tcont;
end

rmse = rmse / N
cont = cont / N
fprintf('Error RMSE = %f, Iteraciones %d', rmse, N);