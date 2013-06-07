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

#define rep(i,n) for(int i=0; i<=(n); i++)

using namespace std;

double temp[100];
double k = (-1.0/20.0);

void fill() {
    temp[0] = 180;
    temp[1] = 175;
    for (int i=1; i<50; i++) {
        temp[i+1] = temp[i] + k*(temp[i] - 80);
    }
}

int main() {
    fill();
    rep(i,30) {
        printf("T[%d] = %.5lf\n",i,temp[i]);
    }
}
