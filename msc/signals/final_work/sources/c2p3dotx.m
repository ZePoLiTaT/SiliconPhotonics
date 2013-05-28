t = [-1:0.01:5];
plot(t,2*rectpuls((t-2)/4));
hold on;
stem(4,-8);
grid on;
xlim([-0.5 5]); ylim([-9 3]);
xlabel('t');
ylabel('Amplitude');
title('$\dot{x}$','interpreter','latex')
