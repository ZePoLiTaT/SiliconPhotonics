clf; clear;

r = 5;
c = [1 0 0; 0 1 0; 0 0 -r^2];

x = [4 4 -1 -1]';
y = sqrt(25-x.^2);
y(2)=-y(2);  y(4)=-y(4);
x = [x y ones(length(x),1)]';

l = c*x;

P = cross(l(:,3),l(:,1));
P = P./P(3);
Q = cross(l(:,2),l(:,4));
Q = Q./Q(3);

l(:,5) = cross(P,Q);
l(:,6) = cross([0 0 1]',[1 0 1]');

R = cross(l(:,5),l(:,6));
R = R./R(3);

%Draw circle
t = linspace(0,2*pi,100)'; 
circsx = r.*cos(t); 
circsy = r.*sin(t); 
plot(circsx,circsy);
hold on;

lx = linspace(-10,10,30);
ly = -l(3,:)./l(2,:) - lx'*l(1,:)./l(2,:);
plot(lx,ly);

lvert = find(l(2,:)==0);
lvert = l(:,lvert);
xvert = -lvert(3,:)./lvert(1,:);
plot([xvert:xvert],[-7:0.1:7],".g");

pts = [P R Q];
scatter(pts(1,:),pts(2,:),5,'r');
scatter(x(1,:),x(2,:),8,'m');

grid;
xlabel('X');
ylabel('Y');
title('Quizz 2');
set(gca(), "xlim", [-7 7]);
set(gca(), "ylim", [-7 7]);
