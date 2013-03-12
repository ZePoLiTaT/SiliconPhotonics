clear ; close all; clc

arg_list = argv();
if nargin < 1
    printf('Filename not found in args, using default [power.dat]\n');
    fname = 'power.dat';
else
    fname = arg_list{1};
endif

printf('Filename: %s\n', fname); 

power = dlmread(fname, ',');

x = 1./(power(:,2));
yt = 10*log(power(:,4)./power(:,3));
yd = 10*log(power(:,5)./power(:,3));

plot(x,yt,"-bo", x, yd, "-rx");
%hold on;
%hold off;
grid;
xlabel('lambda x(10e-6)');
ylabel('power');
title('Z Transmission spectrum');
legend("transmission");
set(gca(), "xlim", [min(x),max(x)]);
print -dpng graph.png

