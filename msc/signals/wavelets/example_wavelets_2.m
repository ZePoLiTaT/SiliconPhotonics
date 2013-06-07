% Load original signal
%load 'freqbrk.mat'; 
%s=freqbrk;
%load 'scddvbrk.mat'; 
%s=scddvbrk;
%load 'cnoislop.mat'; 
%s=cnoislop;
%load 'sumsin.mat'; 
%s=sumsin;
%load 'noispol.mat'; 
%s=noispol;
load 'vonkoch.mat'; 
s=vonkoch;
ls = length(s);
figure(1)
subplot(2,1,1), plot(s)
% Perform decomposition at level 5 of 's' using 'db5'.
[c,l] = wavedec(s,5,'db5');
% Extract approximation coefficients at level 5, 4, 3, 2, and 1
% from wavelet decomposition structure [c,l].
ca5 = appcoef(c,l,'db5',5);
ca4 = appcoef(c,l,'db5',4);
ca3 = appcoef(c,l,'db5',3);
ca2 = appcoef(c,l,'db5',2);
ca1 = appcoef(c,l,'db5',1);
% Extract detail coefficients at levels 1, 2, 3, 4 and 5,
% from wavelet decomposition structure [c,l].
cd5 = detcoef(c,l,5);
cd4 = detcoef(c,l,4);
cd3 = detcoef(c,l,3);
cd2 = detcoef(c,l,2);
cd1 = detcoef(c,l,1);
% Reconstruct approximation at levels 5, 4, 3 ,2 and 1
% from wavelet decomposition structure [c,l].
a5 = wrcoef('a',c,l,'db5',5);
a4 = wrcoef('a',c,l,'db5',4);
a3 = wrcoef('a',c,l,'db5',3);
a2 = wrcoef('a',c,l,'db5',2);
a1 = wrcoef('a',c,l,'db5',1);
% Reconstruct detail coefficients at levels 1,2, 3, 4 and 5, 
% from the wavelet decomposition structure [c,l].
d5 = wrcoef('d',c,l,'db5',5);
d4 = wrcoef('d',c,l,'db5',4);
d3 = wrcoef('d',c,l,'db5',3);
d2 = wrcoef('d',c,l,'db5',2);
d1 = wrcoef('d',c,l,'db5',1);
% Reconstruct 's' from the wavelet decomposition structure [c,l].
a0 = waverec(c,l,'db5');
subplot(2,1,2), plot(a0)
