t = [-3:3];
f = t.^2;

ak = (f(1:end-1) + f(2:end))/2;

ak = [ak ak(end)]
plot(t,f)
hold on
stairs(t,ak,'b')