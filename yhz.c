#include <string.h>
#include <stdio.h>
#include "core.h"

#define versiona 0
#define versionb 0
#define versionc 1

int P_time(char Y_newtime[])
{
  int a;
char Z_s[100], *Z_char;
double Z_d;
strcpy( Z_s, Y_newtime );
Z_s[4] = '\0';
Z_d = strtod( Z_s, &Z_char );
if (Z_d>=PV_lastyear) Z_c_return(11);
  a=strcmp(Y_newtime,PV_shijian);
  if(a==0) return 0;
  if(a>0)
  {
    strcpy(PV_shijian,Y_newtime);
    return 0;
  }
  if(a<0) Z_c_return(10);
}

int P_indebug(char Y_car[], char Y_timein[], char Y_name[])
{
  if (strlen( Y_car )==0) Z_c_return(7);
  if (strlen( Y_timein )==0) Z_c_return(8);
  if (strlen( Y_name ) == 0) Z_c_return(9);

  char Y_temp1[100], Y_temp2[100];
  if(P_carspos(Y_car,Y_temp1,Y_temp2)>0) Z_c_return(12)
  else if (P_time(Y_timein)<0) return -1;
       else return 0;
}

int P_outdebug(char Y_car[], char Y_timeout[], char Y_nameout[])
{
  if (strlen( Y_car )==0) Z_c_return(7);
  if (strlen( Y_timeout )==0) Z_c_return(8);
  if (strlen( Y_nameout )==0) Z_c_return(9);

  char Y_temp1[100], Y_temp2[100];
  if(P_carspos(Y_car,Y_temp1,Y_temp2)<0) return(-14);
  else if (P_time(Y_timeout)<0) return -1;
       else return 0;
}

int P_iinput(char Y_car[], char Y_timein[], char Y_name[], long *Z_pos)
{
  int a,b,c;
  
  c = P_indebug(Y_car,Y_timein,Y_name);
  if(c<0) return c;      
  if(c==0)
  {
    b=P_insert(Y_car,Y_timein,Y_name,Z_pos);
    if(b==0)
    {
      a=P_earn(Y_timein,1.0,1);
      return 0; 
    }
    if(b<0) Z_c_return(13); 
  }
}

int P_oinput(char Y_car[], char Y_timeout[], char Y_nameout[], double *Y_moneymoney)
{
  char Y_timein[13],Y_namein[30];
  int a;
  double Y_money;
  long Y_times,Y_wholetime;
  double Y_wholemoney;
  double Z_p;

  int s,m,e;
        
  s=P_outdebug(Y_car,Y_timeout,Y_nameout);              
  if(s<0) return s;  
  if(s==0)
  {
    e=P_get(Y_car,Y_timein,Y_namein);

    if(e==0)
    {
      P_readout(Y_car,&Y_times,&Y_wholetime,&Y_wholemoney,&Z_p);
      Y_money=P_money(Y_timein,Y_timeout,&Z_p);
      P_readin(Y_car,&Y_money,&Z_p,Y_namein,Y_nameout,Y_timein,Y_timeout);
      P_earn(Y_timeout,Y_money,0);
      P_earn(Y_timeout,1.0,2);
      *Y_moneymoney = Y_money;
      return 0;
    }
  }
}

int P_pdiset(int Y_a0, int Y_a1, int Y_b0, int Y_b1, double Y_c, double Y_d)
{ 
long Z_a, Z_b;
Z_a = Y_a0*60+Y_a1;
Z_b = Y_b0*60+Y_b1;

if (Z_a>Z_b) Z_c_return(5);
        
  PV_disstart[0]=Y_a0;
  PV_disstart[1]=Y_a1;
  PV_disend[0]=Y_b0;
  PV_disend[1]=Y_b1;
  PV_norprice=Y_c;
  PV_disprice=Y_d;
  return 0;
}

int P_adiset(double Y_a, double Y_b)
{

if (Y_a<=Y_b) Z_c_return(6);

  PV_threshold=Y_a;
  PV_gift=Y_b;
  return 0;
}

int P_yhz_c( int Z_c[] )
{

  Z_c[0] = versiona;
  Z_c[1] = versionb;
  Z_c[2] = versionc;
}

