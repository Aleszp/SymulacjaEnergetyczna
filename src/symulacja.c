#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "symulacja.h"

void przygotuj()
{
	//srand(time(NULL));
	srand(1);
}

double gauss(double m, double s) 
{ 
	double x=(double)rand()/RAND_MAX; 
	double y=(double)rand()/RAND_MAX;
	double z1=sqrt(-2.0*log(x))*cos(2.0*3.1415926535*y); 
	
	return s * z1 + m;
}

void segment(double tStart,double tStop,double Pamp,double PpiecStart,double PpiecStop,double* Pmax,double* Wx1,double* W12a,double* W12b,double* W22a,double* W22b,double* W23)
{
	const double tC12a[]={8.0, 11.0, 20.0, 21.0};
	const double tC12b[]={6.0, 13.0, 15.0, 22.0};
	const double tC22a[]={8.0, 11.0, 20.0, 21.0};
	const double tC22b[]={6.0, 21.0};
	const double tC23[]={7.0, 13.0, 19.0, 22.0};
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
		(*Wx1)+=Wtmp;
		//C12a	
		if((t>tC12a[0]&&t<=tC12a[1])||(t>tC12a[2]&&t<=tC12a[3]))
		{
			W12a[0]+=Wtmp;
		}
		else
		{
			W12a[1]+=Wtmp;
		}
		//C12b
		if((t>tC12b[0]&&t<=tC12b[1])||(t>tC12b[2]&&t<=tC12b[3]))
		{
			W12b[0]+=Wtmp;
		}
		else
		{
			W12b[1]+=Wtmp;
		}
		//C22a
		if((t>tC22a[0]&&t<=tC22a[1])||(t>tC22a[2]&&t<=tC22a[3]))
		{
			W22a[0]+=Wtmp;
		}
		else
		{
			W22a[1]+=Wtmp;
		}
		//C22b
		if(t>tC22b[0]&&t<=tC22b[1])
		{
			W22b[0]+=Wtmp;
		}
		else
		{
			W22b[1]+=Wtmp;
		}
		//C23
		if((t>tC23[0]&&t<=tC23[1]))
		{
			W23[0]+=Wtmp;
		}
		else
		{
			if(t>tC23[2]&&t<=tC23[3])
			{
				W23[1]+=Wtmp;
			}
			else
			{
				W23[2]+=Wtmp;
			}
		}
	}
}
