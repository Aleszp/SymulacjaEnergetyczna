/******************************************************************************************************************************
 * 
 * Program symulujący zużycie energii przez mały zakład ceramiczny oraz liczący koszt energii elektrycznej w różnych taryfach.
 * Autor: Aleksander Szpakiewicz-Szatan
 * 09.01.2022
 * 
 ******************************************************************************************************************************/



#ifndef __SYMULACJA_H__
#define __SYMULACJA_H__


void przygotuj();
double gauss(double m, double s);
void segment(int month,double tStart,double tStop,double Pamp,double PpiecStart,double PpiecStop,double* Pmax,double* Wx1,double* W12a,double* W12b,double* W22a,double* W22b,double* W23);
void segment2(int month,double tStart,double tStop,double Pamp,double PpiecStart,double PpiecStop,double* Pmax,double* Wx1,double* W12a,double* W12b,double* W22a,double* W22b,double* W23);

#endif
