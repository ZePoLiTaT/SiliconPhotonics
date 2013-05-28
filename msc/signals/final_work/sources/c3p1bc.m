t1=[-2:0.01:-1];
x1=[2*t1+4];
t2=[-1:0.01:1];
x2=[0*t2+2];
t3=[1:0.01:2];
x3=[-2*t3+4];

t=[t1 t2 t3];
x=[x1 x2 x3];

ws = 3;
tb = [t-ws t t+ws];
xb = [x x x];

plot(tb,xb);
grid on;
ylim([-0.5 2.5]);
xlabel('$\omega$','interpreter','latex');
title('$|X_s(\omega)|$','interpreter','latex');