function [ yo, y, x] = quizz09( varargin )
% Esta funcion genera correspondencias a partir del modelo real y luego las
% contamina con ruido y con datos atipicos para simular que fueron obtenidas
% mediante un experimento real.
%
% SALIDAS :
% yo : Son las observaciones que incluyen ruido y atipicos (outliers).
% y : Son los datos IDEALES. - Que se supone que NO se conocen.
% x : Los valores para los cuales fueron obtenidos 'yo' y 'y'.
%
% Luego :
% (x, yo) Son las correspondencias observadas
% (x, y )  Son las correspondencias ideales
%
% ENTRADAS :
% guardar  :  Primer Argumento     : 0 para NO Guardar, 1 para GUARDAR. - Por defecto = 0
% graficar :   Segundo Argumento : 0 para NO Graficar, 1 para GRAFICAR. - Por defecto = 1
%
% Universidad Tecnologia de Pereira
% Maestria en Ingenieria Electrica
% Vision por Computador
% German A Holguin L

%% Dominio de los datos
x = -20:1:20;

%% Modelo Real
% Que se supone que NO conocemos.
a = 1; b = 0.4; c = 0.8;
y = a + b*sin(x/10) + c*sin(x/20);

%% Generar ruido de la forma N(0,sg^2)
% Esto es, Guasiano con media cero y desviacion estandar sigma (sg).
sg = 0.1;
r = sg*randn(1,length(x));

%% Datos Observados por el sensor.  ( Es decir, con ruido )
% Aunque aqui todavia NO hay datos atipicos ( outliers )
yr = y + r;

%% Generar aleatoriamente del 10% al 20% de correspondencias falsas
% numero de outliers
No = ceil ( 0.1*length(x) + 0.1*length(x)*rand(1) ) ;
% Cuales ? - Seleccionarlos aleatoriamente
outliers = randperm( length(x), No);
% A estos seleccionados como outliers, sumarles o restarles aleatoriamente
% M sigmas. En una Gausiana M>5 sigmas, es atipico.
M = 6;
% Generar aleatoriamente el signo del dato atipico:
% y que esto funcione igual tanto en Matlab como en Octave
sel = [ -1 , 1];
for k = 1:No
    indxsel = randperm(2,1);
    signos(k) = sel(indxsel);
end
% Inicializar los datos de salida con los datos ruidosos
yo = yr;
% y ahora si, adicionar los atípicos con el signo aleatorio
yo(outliers) = yr(outliers) + M*sg*signos(1,:);

%% Procesar los argumentos
switch nargin
    case 1   % Un solo argumento, el primero, que es guardar.
        guardar = varargin{1};
        graficar = 0;
    case 2  % Dos argumentos.
        guardar = varargin{1};
        graficar = varargin{2};
    otherwise  % Sin argumentos, o con mas de dos argumentos. Por defecto.
        guardar = 0;
        graficar = 1;
end

%% Guardar las correspondencias
if  guardar ~= 0
    % Este archivo contendra TRES filas.  La primera fila son las Xs, la
    % segunda los Y reales, y la tercera los YO observados con outliers.
    save('Quizz09data.csv','x','y','yo','-ascii','-tabs');
    %
    % Esto no es necesario para aquellos trabajando directamente en Matlab
    % o en Octave, pues la funcion ya devuelve estos valores.
    %
    % Para aquellos trabajando en C/C++, pueden usar este archivo ASCII
    % para leerlo desde su programa y no tener que reimplementar esta parte
    % en su codigo.
    % 
    % NOTA: En LINUX este script se puede ejectuar directamente desde su codigo
    % C/C++ haciendo una llamada de sistema a un script con permisos de ejecucion,
    % que corra la funcion con los parametros adecuados.  Se anexa el ejemplo
    % runq09Linux, que es un ejemplo de octave batch script.
end

%% Graficas
if  graficar ~= 0
    figure(100);
    clf;
    % Los datos ideales
    plot(x,y,'b.-');
    hold on;
    grid on;
    % Los datos con ruido del sensor
    plot(x,yr,'r+');
    % Los datos observados con outliers
    plot(x,yo,'mo-');
    % Etiquetas de cada plot
    legend('Ideales','con ruido','con atipicos');
end

