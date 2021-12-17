/****************************************
 * 
 * Program symulujący zużycie energii przez mały zakład ceramiczny oraz liczący koszt energii elektrycznej w różnych taryfach.
 * Autor: Aleksander Szpakiewicz-Szatan
 * 17.12.2021
 * 
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double gauss(double m, double s);
void segment(double tStart,double tStop,double Pamp,double PpiecStart,double PpiecStop,double* Pmax,double* Wx1,double* W12a,double* W12b,double* W22a,double* W22b,double* W23);

int main(void)
{
	//srand(time(NULL));
	srand(1);
	const double Prozruch=50.0;
	const double Ppodtrzymanie=35.0;
	const double Pchłodzenie=20.0;
	const double Kdzien=2.0;
	const double Knoc=0.5;
	const double biuro=0.2;
	const double tokarka=1.5;
	const double frezarka=2.0;
	const double przecinarka=2.0;
	const double oswietlenie=0.3;
	const double prasa=2.0;
	
	const double tPolnoc1=0.0;
	const double tOtwarcie=8.0;
	const double tRozruch=9.0;
	const double tPodtrzymanie=12.0;
	const double tChlodzenie=14.0;
	const double tZmiana=16.0;
	const double tStop=20.0;
	const double tPolnoc2=24.0;
	
	//const int dni=252;
	
	double 	  Pamp=0.0;
	double    Pmax=0.0;

	
	double     Wx1=0.0;
	double  W12a[]={0.0,0.0};
	double  W12b[]={0.0,0.0};
	double  W22a[]={0.0,0.0};
	double  W22b[]={0.0,0.0};
	double   W23[]={0.0,0.0,0.0};
	
	const double   C11=0.5857;
	const double C12a[]={0.7083,0.4590};
	const double C12b[]={0.6829,0.4496};
	
	const double C21=0.5498;
	const double C22a[]={0.6870,0.4745};
	const double C22b[]={0.6179,0.3851};
	const double C23[]={0.6746,0.7569,0.4545};
	
	//double sprawnosc=1.0;
	
	
	//void segment(double tStart,double tStop,double Pamp,double PpiecStart,double PpiecStop,double* Pmax,double* Wx1,double* W12a,double* W12b,double* W22a,double* W22b,double* W23);
	Pamp=Knoc+oswietlenie;
	segment(tPolnoc1,tOtwarcie,Pamp,0.0,0.0,&Pmax,&Wx1,W12a,W12b,W22a,W22b,W23);
	
	Pamp=Kdzien+biuro+tokarka+frezarka+przecinarka+oswietlenie+prasa;
	segment(tOtwarcie,tRozruch,Pamp,0.0,0.0,&Pmax,&Wx1,W12a,W12b,W22a,W22b,W23);
	segment(tRozruch,tPodtrzymanie,Pamp,Prozruch,Prozruch,&Pmax,&Wx1,W12a,W12b,W22a,W22b,W23);
	segment(tPodtrzymanie,tChlodzenie,Pamp,Ppodtrzymanie,Ppodtrzymanie,&Pmax,&Wx1,W12a,W12b,W22a,W22b,W23);
	segment(tChlodzenie,tZmiana,Pamp,Pchłodzenie,Pchłodzenie*(tStop-tZmiana)/(tStop-tChlodzenie),&Pmax,&Wx1,W12a,W12b,W22a,W22b,W23);
	
	Pamp=Knoc+oswietlenie;
	segment(tZmiana,tStop,Pamp,Pchłodzenie*(tStop-tZmiana)/(tStop-tChlodzenie),0.0,&Pmax,&Wx1,W12a,W12b,W22a,W22b,W23);
	segment(tStop,tPolnoc2,Pamp,0.0,0.0,&Pmax,&Wx1,W12a,W12b,W22a,W22b,W23);
	
	printf("Moc maksymalna, %0.4lf\n",Pmax);
	
	printf("Dzienne zużycie [kWh]\n");
	printf("Cx1,C12aSzczyt,C12aPozaszczyt,C12bSzczyt,C12bPozaszczyt,C22aSzczyt,C22aPozaszczyt,C22bSzczyt,C22bPozaszczyt,C23Szczyt1,C23Szczyt2,C23Pozaszczyt\n");
	printf("%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n\n",Wx1, W12a[0],W12a[1], W12b[0],W12b[1], W22a[0],W22a[1], W22b[0],W22b[1], W23[0],W23[1],W23[2]);
	
	
	printf("Stawka [zł/kWh]:\n");
	printf("C11,C12aSzczyt,C12aPozaszczyt,C12bSzczyt,C12bPozaszczyt,C21,C22aSzczyt,C22aPozaszczyt,C22bSzczyt,C22bPozaszczyt,C23Szczyt1,C23Szczyt2,C23Pozaszczyt\n");
	printf("%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n\n",C11, C12a[0],C12a[1], C12b[0],C12b[1], C21, C22a[0],C22a[1], C22b[0],C22b[1], C23[0],C23[1],C23[2]);
	
	
	printf("Dzienny koszt [zł]:\n");
	printf("C11,C12a,C12b,C21,C22a,C22b,C23\n");
	printf("%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n",Wx1*C11,C12a[0]*W12a[0]+C12a[1]*W12a[1],C12b[0]*W12b[0]+C12b[1]*W12b[1],Wx1*C21,C22a[0]*W22a[0]+C22a[1]*W22a[1],C22b[0]*W22b[0]+C22b[1]*W22b[1],C23[0]*W23[0]+C23[1]*W23[1]+C23[2]*W23[2]);
	
	return 0;
}

double gauss(double m, double s) 
{ 
	double x=(double)rand()/RAND_MAX; 
	double y=(double)rand()/RAND_MAX; // Dwie liczby o rozkładzie normalnym 
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
