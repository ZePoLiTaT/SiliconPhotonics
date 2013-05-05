from sys import stdin
from math import *

def ispal(sdig):
	l = len(sdig);
	for i in xrange( int(l/2) ):
		if(sdig[i] != sdig[l-i-1]):
			return False;
	return True;


a={0:0}
T=int(1e14)
count = 0
for i in xrange(1,1+int(sqrt(T))):
	i2 = i*i;
	if(ispal(str(i)) and ispal(str(i2))):
		count = count + 1;
		a[i*i] = count;
print a
