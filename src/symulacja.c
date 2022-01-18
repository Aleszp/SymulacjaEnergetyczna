/******************************************************************************************************************************
 * 
 * Program symulujący zużycie energii przez mały zakład ceramiczny oraz liczący koszt energii elektrycznej w różnych taryfach.
 * Autor: Aleksander Szpakiewicz-Szatan
 * 09.01.2022
 * 
 ******************************************************************************************************************************/


#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "symulacja.h"

//Funkcja sadząca ziarno liczb losowych (by w głównej części programu nie trzeba było załączać STDLIB)
void przygotuj()
{
	//srand(time(NULL));
	srand(1);
}

//Liczba pseudolosowa o rozkładzie gaussa, m - wartość średnia, s - odchylenie standardowe
double gauss(double m, double s) 
{ 
	double x=(double)rand()/RAND_MAX; 
	double y=(double)rand()/RAND_MAX;
	double z1=sqrt(-2.0*log(x))*cos(2.0*3.1415926535*y); 
	
	return s * z1 + m;
}

//Symulacja zużycia mocy dla czasu od tStart do tStop
void segment(int month,double tStart,double tStop,double Pamp,double PpiecStart,double PpiecStop,double* Pmax,double* Wx1,double* W12a,double* W12b,double* W22a,double* W22b,double* W23)
{
	const double tC12a_z[]={8.0, 11.0, 17.0, 21.0};
	const double tC12a_l[]={8.0, 11.0, 20.0, 21.0};
	const double tC12b[]={6.0, 13.0, 15.0, 22.0};
	const double tC22a[12][4]=
	{
		{8.0, 11.0, 16.0, 21.0},
		{8.0, 11.0, 16.0, 21.0},
		{8.0, 11.0, 18.0, 21.0},
		{8.0, 11.0, 19.0, 21.0},
		{8.0, 11.0, 20.0, 21.0},
		{8.0, 11.0, 20.0, 21.0},
		{8.0, 11.0, 20.0, 21.0},
		{8.0, 11.0, 20.0, 21.0},
		{8.0, 11.0, 20.0, 21.0},
		{8.0, 11.0, 19.0, 21.0},
		{8.0, 11.0, 18.0, 21.0},
		{8.0, 11.0, 16.0, 21.0}
	};
		
	const double szczytMocowy[]={7.0,22.0};
	const double tC22b[]={6.0, 21.0};
	const double tC23_l[]={7.0, 13.0, 19.0, 22.0};
	const double tC23_z[]={7.0, 13.0, 16.0, 21.0};
	
	const double dt=1.0/60.0;
	
	double Wtmp=0.0;
	double Ptmp=0.0;
	double Ppiec=PpiecStart;
	
	for(double t=tStart;t<tStop;t+=dt)
	{
		Ppiec=PpiecStart+(PpiecStop-PpiecStart)*(t-tStart)/(tStop-tStart);
		Ptmp=gauss(0.75*Pamp,0.25*Pamp)+Ppiec;
		if(Ptmp>(*Pmax))
			(*Pmax)=Ptmp;
		Wtmp=dt*Ptmp;
		
		//C11,C21
		Wx1[0]+=Wtmp;
		if(Wtmp>40.0*dt)
			Wx1[1]+=Wtmp-40.0*dt;
		
		if((t>szczytMocowy[0]&&t<=szczytMocowy[1]))
		{
			Wx1[2]+=Wtmp;
		}	
			
		//C12a	
		if(month>3&&month<9)	//lato
		{
			if((t>tC12a_l[0]&&t<=tC12a_l[1])||(t>tC12a_l[2]&&t<=tC12a_l[3]))
			{
				W12a[0]+=Wtmp;
				if(Wtmp>50.0*dt)
					W12a[2]+=Wtmp-50.0*dt;
			}
			else
			{
				W12a[1]+=Wtmp;
				if(Wtmp>40.0*dt)
					W12a[3]+=Wtmp-40.0*dt;
			}
		}
		else	//zima
		{
			if((t>tC12a_z[0]&&t<=tC12a_z[1])||(t>tC12a_z[2]&&t<=tC12a_z[3]))
			{
				W12a[0]+=Wtmp;
				if(Wtmp>50.0*dt)
					W12a[2]+=Wtmp-50.0*dt;
			}
			else
			{
				W12a[1]+=Wtmp;
				if(Wtmp>40.0*dt)
					W12b[3]+=Wtmp-40.0*dt;
			}
		}
		//C12b
		if((t>tC12b[0]&&t<=tC12b[1])||(t>tC12b[2]&&t<=tC12b[3]))
		{
			W12b[0]+=Wtmp;
			if(Wtmp>50.0*dt)
				W12b[2]+=Wtmp-50.0*dt;
			
		}
		else
		{
			W12b[1]+=Wtmp;
		}
		//C22a
		if((t>tC22a[month][0]&&t<=tC22a[month][1])||(t>tC22a[month][2]&&t<=tC22a[month][3]))
		{
			W22a[0]+=Wtmp;
			if(Wtmp>50.0*dt)
				W22a[2]+=Wtmp-50.0*dt;
		}
		else
		{
			W22a[1]+=Wtmp;
		}
		//C22b
		if(t>tC22b[0]&&t<=tC22b[1])
		{
			W22b[0]+=Wtmp;
			if(Wtmp>50.0*dt)
				W22b[2]+=Wtmp-50.0*dt;
		}
		else
		{
			W22b[1]+=Wtmp;
		}
		//C23
		//lato
		if(month>3&&month<9)
		{
			if((t>tC23_l[0]&&t<=tC23_l[1]))
			{
				W23[0]+=Wtmp;
				if(Wtmp>50.0*dt)
					W23[3]+=Wtmp-50.0*dt;
			}
			else
			{
				if(t>tC23_l[2]&&t<=tC23_l[3])
				{
					W23[1]+=Wtmp;
					if(Wtmp>50.0*dt)
						W23[3]+=Wtmp-50.0*dt;
				}
				else
				{
					W23[2]+=Wtmp;
				}
			}
		}
		else	//zima
		{
			if((t>tC23_z[0]&&t<=tC23_z[1]))
			{
				W23[0]+=Wtmp;
				if(Wtmp>50.0*dt)
					W23[3]+=Wtmp-50.0*dt;
			}
			else
			{
				if(t>tC23_z[2]&&t<=tC23_z[3])
				{
					W23[1]+=Wtmp;
					if(Wtmp>50.0*dt)
						W23[4]+=Wtmp-50.0*dt;
				}
				else
				{
					W23[2]+=Wtmp;
				}
			}
		}
	}
}
