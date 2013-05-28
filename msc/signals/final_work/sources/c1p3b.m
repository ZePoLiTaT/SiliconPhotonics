fe = [-2*pi*751000 -2*pi*749000 2*pi*749000 2*pi*751000];
Ae = [1 1 1 1];
pe = [0 0 0 0];

figure(1); 
subplot(2,2,1), stem(fe,Ae);
grid on;
xlim([-2*pi*752000 -2*pi*748000]);
set(gca,'XTick',-2*pi*752000:2*pi*1000:-2*pi*748000)
set(gca,'XTickLabel',{'','-2*pi*751000','','-2*pi*749000',''})
xlabel('Frec [rad/s]');
ylabel('Amplitude');
title('|X_n|', 'fontweight', 'bold');

subplot(2,2,2), stem(fe,Ae);
grid on;
xlim([2*pi*748000 2*pi*752000]);
set(gca,'XTick',2*pi*748000:2*pi*1000:2*pi*752000)
set(gca,'XTickLabel',{'','2*pi*748000','','2*pi*751000',''})
xlabel('Frec [rad/s]');
title('|X_n|', 'fontweight', 'bold');

%zoom xon;
%text(-2*pi*751000,0.5,'\leftarrow sin(-\pi\div4)', 'HorizontalAlignment','right')
%text(-2*pi*749000,0.5,'\leftarrow sin(-\pi\div4)', 'HorizontalAlignment','left')
%set(gca,'ytick',[])

subplot(2,2,3:4), stem(fe,pe);
grid on;
xlabel('Frec [rad/s]');
ylabel('Phase');
title('$\angle x$','interpreter','latex');