/*
 * * =================================================
 * *
 * *       Filename:  main.cpp
 * *
 * *    Description:  Calculate Temperature using
 * *				  Newton's Law of Cooling
 * *
 * *        Version:  1.0
 * *        Created:  01/19/09 20:26:43
 * *       Compiler:  g++
 * *
 * *         Author:  Sebastián Gomez
 * *				  Tatiana López G
 * *         Company: Universidad Tecnológica de Pereira
 * *
 * * =================================================
 * */

#include <cstdio>
#include <cstdlib>
#include <algorithm>

#define rep(i,n) for(int i=0; i<(n); i++)
#define repf(i,a,b) for (int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=(b); i--)

using namespace std;

double x(int n) {
	return (n>=0)?1.0:0.0; //unitary step
	//return (n==0)?1.0:0.0; //impulse
}

double dpa[100];

double a(int n) {
	if (n<0) return 0.0;
	if (dpa[n] > 0) return dpa[n];
	return dpa[n] = x(n) + 0.9*a(n-1);
}

double y(int n) {
	return a(n) + 2.0*a(n-1) + 3.0*a(n-2);
}

int main() {
	rep(i,100) dpa[i] = -1;
	rep(i,10) {
		printf("y[%d] = %.5lf\n",i,y(i));
	}
}
