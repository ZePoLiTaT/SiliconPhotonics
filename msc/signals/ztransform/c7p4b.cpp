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
 * *         Author:  Tatiana López G
 * *         Company: Universidad Tecnológica de Pereira
 * *
 * * =================================================
 * */

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <math.h>

#define rep(i,n) for(int i=0; i<(n); i++)

using namespace std;

double x(int n) {
	return (n>=0)?1.0:0.0; //unitary step
}

double y(int n) {
	if(n<0) return 0;
	return 0.9*y(n-1) + x(n) + 2*x(n-1) + 3*x(n-2);
}

int main() {
	rep(i,5) {
		printf("y[%d] = %.5lf\n",i,y(i));
	}
}
