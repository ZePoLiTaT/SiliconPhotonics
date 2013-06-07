/*
 * ==========================================================
 *
 *       Filename:  haar.cpp
 *
 *    Description:  Discrete Haar Transform Implementation
 *
 *        Version:  1.0
 *        Created:  30/05/13 06:14:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Tatiana López G (ZePoLiTaT), tatiana@sirius.utp.edu.co
 *        Company:  Grupo de Investigación Sirius
 *
 * ============================================================
 */
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <map> 

void printm(float *m, int N)
{
	printf("\n>> [");
	for(int i=0; i<N; i++)
		printf("%.4f ",m[i]);
	printf("]");
}

void haar(float *donees, float *sdetail, int N)
{
	float moyenne, difference;

	int NHalf = N>>1;
	int ti;

	N = NHalf;
	while(N>0)
	{
		//printf("\nN:%d\n",N);
		for (int i=0; i<N; i++)
		{
			ti = i<<1;	
			moyenne = (donees[ti] + donees[ti+1]) / 2.0f;	
			difference = donees[ti] - moyenne;

			sdetail[N + i] = difference;
			donees[i] = moyenne;
			//printf("%.4f %.4f\n", moyenne, difference);
		}
		N = N>>1;	//shift right by 1 bit is the same as divide by 2
	}

	sdetail[0] = moyenne;
}

int main ( int argc, char *argv[] )
{
	int N;
	float *donees;
	float *sdetail;

	//Read N from standard input
	scanf("%d", &N);

	//Create arrays of size N
	donees = (float *)malloc(N* sizeof(*donees));
	sdetail = (float *)malloc(N* sizeof(*sdetail));

	//Fill the values of initial array from stdin
	for(int i=0; i<N; i++)
	{
		scanf("%f", &donees[i]);
	}

	printm(donees, N);
	haar(donees, sdetail, N);
	printm(sdetail, N);

	delete donees;
	delete sdetail;	

	return EXIT_SUCCESS;
}		
