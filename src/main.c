/******************************************************************************************************************************
 * 
 * Program symulujący zużycie energii przez mały zakład ceramiczny oraz liczący koszt energii elektrycznej w różnych taryfach.
 * Autor: Aleksander Szpakiewicz-Szatan
 * 09.01.2022
 * 
 ******************************************************************************************************************************/

#include <stdio.h>

#include "symulacja.h"

int main(void)
{
	przygotuj();
	
	//Duża ilość stałych wartości (do zastąpienia importem z bazy danych?)
	
	//Stałe dotyczące mocy pieca [kW]
	const double Prozruch=50.0;
	const double Ppodtrzymanie=35.0;
	const double Pchłodzenie=20.0;
	
	//Stałe dotyczące mocy pozostałych urządzeń [kW]
	const double Kdzien=2.0;
	const double Knoc=0.5;
	const double biuro=0.2;
	const double tokarka=1.5;
	const double frezarka=2.0;
	const double przecinarka=2.0;
	const double oswietlenie=0.3;
	const double prasa=2.0;
	
	//Czasy poszczególnych etapów pracy zakładu
	const double tPolnoc1=0.0;
	const double tOtwarcie=8.0;
	const double tRozruch=9.0;
	const double tPodtrzymanie=12.0;
	const double tChlodzenie=14.0;
	const double tZmiana=16.0;
	const double tStop=20.0;
	const double tPolnoc2=24.0;
	
	//Zmienne pomocnicze
	double 	  Pamp=0.0;
	double    Pmax=0.0;
	
	//Stawki w poszczególnych taryfach z rozdzieleniem między szczyt(y) i nie-szczyt
	//Taryfy do 40kW
	const double   C11=0.5857;
	const double C12a[]={0.7083,0.4590};
	const double C12b[]={0.6829,0.4496};
	
	//Taryfy ponad 40kW
	const double C21=0.5498;
	const double C22a[]={0.6870,0.4745};
	const double C22b[]={0.6179,0.3851};
	const double C23[]={0.6746,0.7569,0.4545};
	
	const double oplataMocowa=0.10;
	
	int day=3;
	int month=0;

	const int daymax=4;
	
	FILE* out1=stdout;
	FILE* out2=stderr;

	fprintf(out1,"Dzienne zużycie [kWh]\n");
	fprintf(out1,"miesiac,dzien,Cx1,C11Ponad40kW,C12aSzczyt,C12aPozaszczyt,C12aPonad50kW,C12aPonad40kW,C12bSzczyt,C12bPozaszczyt,C12bPonad50kW,C12bPonad40kW,C22aSzczyt,C22aPozaszczyt,C22aPonad50kW,C22bSzczyt,C22bPozaszczyt,C22bPonad50kW,C23Szczyt1,C23Szczyt2,C23Pozaszczyt,C23Ponad50kW1,C23Ponad50kW2,opłataMocowa\n");
	fprintf(stderr,"godzina,Cx1,C11Ponad40kW,C12aSzczyt,C12aPozaszczyt,C12aPonad50kW,C12aPonad40kW,C12bSzczyt,C12bPozaszczyt,C12bPonad50kW,C12bPonad40kW,C22aSzczyt,C22aPozaszczyt,C22aPonad50kW,C22bSzczyt,C22bPozaszczyt,C22bPonad50kW,C23Szczyt1,C23Szczyt2,C23Pozaszczyt,C23Ponad50kW1,C23Ponad50kW2\n");
	
	////fprintf(out2,"Dzienny koszt [zł]:\n");
	//fprintf(out2,"miesiac,dzien,C11,C12a,C12b,C21,C22a,C22b,C23,opłataMocowa\n");
		
	while(day<daymax)
	{	
		switch(day)
		{
			case 31:	//koniec stycznia
			case 59:
			case 90:
			case 120:
			case 151:
			case 181:
			case 212:
			case 243:
			case 273:
			case 304:
			case 334:
			case 365:
				month++;
				break;
		}
		
		
		//Zużycie energii podzielone dla poszczególnych taryf z rozdzieleniem zużycia między szczyt(y) i nie-szczyt
		double   Wx1[]={0.0,0.0,0.0};
		double  W12a[]={0.0,0.0,0.0,0.0};
		double  W12b[]={0.0,0.0,0.0,0.0};
		double  W22a[]={0.0,0.0,0.0};
		double  W22b[]={0.0,0.0,0.0};
		double   W23[]={0.0,0.0,0.0,0.0,0.0};
		
		if(day%6==0 || day%7==0)	//brak pracy w weekendy
		{
			Pamp=Knoc+oswietlenie;
			segment(month,tPolnoc1,tPolnoc2,Pamp,0.0,0.0,&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
			
			//Dzienne zużycie
			fprintf(out1,"%i,%i,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n",month,day,Wx1[0],Wx1[1], W12a[0],W12a[1],W12a[2],W12a[3], W12b[0],W12b[1],W12b[2],W12a[3], W22a[0],W22a[1],W22a[2], W22b[0],W22b[1],W22b[2], W23[0],W23[1],W23[2],W23[3],W23[4],Wx1[2]);	
			//Dzienny koszt
			//fprintf(out2,"%i,%i,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n",month,day,Wx1[0]*C11,C12a[0]*W12a[0]+C12a[1]*W12a[1],C12b[0]*W12b[0]+C12b[1]*W12b[1],Wx1[0]*C21,C22a[0]*W22a[0]+C22a[1]*W22a[1],C22b[0]*W22b[0]+C22b[1]*W22b[1],C23[0]*W23[0]+C23[1]*W23[1]+C23[2]*W23[2],Wx1[2]*oplataMocowa);
			
			day++;
			continue;
		}
		
		//od północy do otwarcia - tylko oświetlenie i klimatyzacja
		Pamp=Knoc+oswietlenie;
		segment2(month,tPolnoc1,tOtwarcie,Pamp,0.0,0.0,&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
		
		//od otwarcia do zamknięcia uwzględniaj różne urządzenia
		Pamp=Kdzien+biuro+tokarka+frezarka+przecinarka+oswietlenie+prasa;
		
		//Czas do załadowania (i uruchomienia) pieca
		segment2(month,tOtwarcie,tRozruch,Pamp,0.0,0.0,&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
		
		//Podgrzewanie
		segment2(month,tRozruch,tPodtrzymanie,Pamp,Prozruch,Prozruch,&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
		
		//Podtrzymanie
		segment2(month,tPodtrzymanie,tChlodzenie,Pamp,Ppodtrzymanie,Ppodtrzymanie,&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
		
		//Chłodzenie
		segment2(month,tChlodzenie,tZmiana,Pamp,Pchłodzenie,Pchłodzenie*(tStop-tZmiana)/(tStop-tChlodzenie),&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
		
		//Koniec pracy w zakładzie, dalsze chłodzenie (tylko stygnący piec, oświetlenie i klimatyzacja)
		Pamp=Knoc+oswietlenie;
		segment2(month,tZmiana,tStop,Pamp,Pchłodzenie*(tStop-tZmiana)/(tStop-tChlodzenie),0.0,&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
		
		//Koniec schłodzenia, brak pracy w zakładzie (tylko oświetlenie i klimatyzacja)
		segment2(month,tStop,tPolnoc2,Pamp,0.0,0.0,&Pmax,Wx1,W12a,W12b,W22a,W22b,W23);
		
		//Podaj wyniki:
		
		
		//Dzienne zużycie
		fprintf(out1,"%i,%i,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n",month,day,Wx1[0],Wx1[1], W12a[0],W12a[1],W12a[2],W12a[3], W12b[0],W12b[1],W12b[2],W12a[3], W22a[0],W22a[1],W22a[2], W22b[0],W22b[1],W22b[2], W23[0],W23[1],W23[2],W23[3],W23[4],Wx1[2]);	
		//Dzienny koszt
		//fprintf(out2,"%i,%i,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n",month,day,Wx1[0]*C11,C12a[0]*W12a[0]+C12a[1]*W12a[1],C12b[0]*W12b[0]+C12b[1]*W12b[1],Wx1[0]*C21,C22a[0]*W22a[0]+C22a[1]*W22a[1],C22b[0]*W22b[0]+C22b[1]*W22b[1],C23[0]*W23[0]+C23[1]*W23[1]+C23[2]*W23[2],Wx1[2]*oplataMocowa);

		day++;
	}
	
	//Moc maksymalna
	fprintf(out1,"Moc maksymalna, %0.4lf\n",Pmax);
	//Obowiązujące stawki
	//printf("Stawka [zł/kWh]:\n");
	//printf("C11,C12aSzczyt,C12aPozaszczyt,C12bSzczyt,C12bPozaszczyt,C21,C22aSzczyt,C22aPozaszczyt,C22bSzczyt,C22bPozaszczyt,C23Szczyt1,C23Szczyt2,C23Pozaszczyt\n");
	//printf("%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n\n",C11, C12a[0],C12a[1], C12b[0],C12b[1], C21, C22a[0],C22a[1], C22b[0],C22b[1], C23[0],C23[1],C23[2]);
	
	//Dzienny koszt
	//printf("Dzienny koszt [zł]:\n");
	//printf("C11,C12a,C12b,C21,C22a,C22b,C23\n");
	//printf("%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf,%0.4lf\n",Wx1[0]*C11,C12a[0]*W12a[0]+C12a[1]*W12a[1],C12b[0]*W12b[0]+C12b[1]*W12b[1],Wx1[0]*C21,C22a[0]*W22a[0]+C22a[1]*W22a[1],C22b[0]*W22b[0]+C22b[1]*W22b[1],C23[0]*W23[0]+C23[1]*W23[1]+C23[2]*W23[2]);
	
	return 0;
}
