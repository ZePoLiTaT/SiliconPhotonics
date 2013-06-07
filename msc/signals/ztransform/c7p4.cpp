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

double d(int n) {
	return (n==0)?1.0:0.0; //impulse
}

double u(int n) {
	return (n>=0)?1.0:0.0; //unitary step
}

double y(int n) {
	return 3.33*d(n) + 60*u(n) -62.33*pow(0.9,n);
}

int main() {
	rep(i,5) {
		printf("y[%d] = %.5lf\n",i,y(i));
	}
}
