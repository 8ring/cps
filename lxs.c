#include <stdio.h>
#include <string.h>
#include "core.h"

#define versiona 0
#define versionb 0
#define versionc 1

double Z_period( long Z_q, long Z_h )
{
  long Z_qq, Z_hh;
  Z_qq=60*PV_disstart[0]+PV_disstart[1];
  Z_hh=60*PV_disend[0]+PV_disend[1];
  if ((Z_qq>Z_q) && (Z_qq<Z_h)) return( Z_period( Z_q, Z_qq )+ Z_period( Z_qq, Z_h ) );
  if ((Z_hh<Z_h) && (Z_hh>Z_q)) return( Z_period( Z_q, Z_hh )+ Z_period( Z_hh, Z_h ) );
  if ((Z_qq<=Z_q) && (Z_hh>=Z_h)) return( (Z_h-Z_q) * PV_disprice );
  return( (Z_h-Z_q) * PV_norprice );
}
 
double Z_double_double( double Z_d )
{
  char Z_s[100], *Z_temp;
  long i, o, p;
  double dd;
  sprintf( Z_s, "%lf", Z_d);
  p = strlen(Z_s);
  for (i=0;i<=p;i++)
    if (Z_s[i]=='.') Z_s[i] = '\0';
  dd = strtod( Z_s, &Z_temp );
  return(dd);
}

int L_cuttime(char L_time[13], int *L_year, int *L_month, int *L_day, int *L_hour, int *L_min)
{
  int a,b,c,d,e;
  a=1000*(L_time[0]-'0')+100*(L_time[1]-'0')+10*(L_time[2]-'0')+(L_time[3]-'0');
  b=10*(L_time[4]-'0')+(L_time[5]-'0');
  c=10*(L_time[6]-'0')+(L_time[7]-'0');
  d=10*(L_time[8]-'0')+(L_time[9]-'0');
  e=10*(L_time[10]-'0')+(L_time[11]-'0');

  *L_year=a;
  *L_month=b;
  *L_day=c;
  *L_hour=d;
  *L_min=e;
}

double L_discount(char *L_in,char *L_out)
{
  long gap;  
  int y[2],m[2],d[2],h[2],min[2];
  long tt1[2]={0,0},tt0[2]={0,0};
  double daycost,sum,sum1,sum2;
  int a,b,n;

  L_cuttime(L_in,&y[0],&m[0],&d[0],&h[0],&min[0]);
  L_cuttime(L_out,&y[1],&m[1],&d[1],&h[1],&min[1]);
  gap=P_gap(L_in,L_out);
     
  if (y[0]==y[1] && m[0]==m[1] && d[0]==d[1] &&(h[1]*60+min[1]-h[0]*60-min[0])<=PV_free) return 0;
  else  
  {
    tt0[0]=60*PV_disstart[0]+PV_disstart[1];
    tt0[1]=60*PV_disend[0]+PV_disend[1];
    tt1[0]=60*h[0]+min[0];
    tt1[1]=60*h[1]+min[1];        
    daycost=PV_norprice*(24.0*60.0-tt0[1]+tt0[0])+PV_disprice*(tt0[1]-tt0[0]);

    if (y[0] == y[1] && m[0] == m[1] && d[0] == d[1])
    {
      return Z_period( tt1[0], tt1[1] );
    } else
    {
      sum1 = Z_period( tt1[0], 24 * 60 );
      sum2 = Z_period( 0, tt1[1] );
      a = 24 * 60 - tt1[0];
      b = tt1[1];        
      n=(gap-a-b)/(24*60);
      sum=sum1+sum2+n*daycost;
      return sum;
    }
  }
}

long P_gap(char *time1,char *time2)
{
  int y[2],m[2],d[2],h[2],min[2];
  int i,j,c;
  long gap;
  int days = 0,daysum = 0,day[2]={0,0}; 
  int n[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
  L_cuttime(time1,&y[0],&m[0],&d[0],&h[0],&min[0]);
  L_cuttime(time2,&y[1],&m[1],&d[1],&h[1],&min[1]);

  for(i=y[1]-y[0];i>1;i--)
  {
    c=L_year(y[1]-i+1);
    if(c==0) days+=365;
    else days+=366;
  }
  for(j=12;j>=1;j--)
  {
    if(m[0]<j) day[0]+=n[j];
    else break;
  }

  day[0]+=n[j]-d[0];
  c=L_year(y[0]);
  if(c==1 && m[0]<=2) day[0]++;

  for(j=1;j<=12;j++)
  { 
    if (m[1]>j) day[1]+=n[j];
    else break;
  }
  day[1]+=d[1];
  c=L_year(y[1]);
  if (c==1 && m[1]>2) day[1]++;

  daysum=days+day[0]+day[1]-1;
  if (y[0]==y[1]&&c==1) daysum=daysum-366;
  if(y[0]==y[1]&&c==0) daysum=daysum-365;
  gap=24*60-60*h[0]-min[0]+60*h[1]+min[1]+daysum*24*60;
  return gap; 
}

double L_gift(double *P,double discount)
{
  double a, b, Z_c;
  double gift;
  if ((discount==0)||((*P)<PV_threshold)) 
  {
    (*P) = (*P) + discount;
    return 0;
  }
  else
  {
    a=Z_double_double(discount/PV_gift);
    b=Z_double_double((*P)/PV_threshold);
    if (b*PV_gift<=discount)
    {
      gift=b*PV_gift;
      (*P)=(*P)-b*PV_threshold+discount-gift;
    } else
    {
      gift=discount;
      (*P)=(*P)-(a)*PV_threshold;
      if ((discount - a * PV_gift)>0.00001) (*P)=(*P)-PV_threshold; 
    }
  } 
  return gift;
}

int L_year(int year)
{
  if( (year%4) !=0) return 0;
  else if( (year%400) !=0 && (year%100)==0) return 0;
  else return 1;
}

double P_money(char *L_in,char *L_out,double *P)
{
  double discount,gift,sum;

  discount=L_discount(L_in,L_out);
  gift=L_gift(P,discount);
  sum=discount-gift;
  return sum;
}

int P_lxs_c( int Z_c[] )
{

  Z_c[0] = versiona;
  Z_c[1] = versionb;
  Z_c[2] = versionc;
}

