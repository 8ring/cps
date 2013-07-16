#include <string.h>
#include <stdio.h>
#include "core.h"

#define versiona 0
#define versionb 0
#define versionc 1

void Z_f_readtime( long Z_k )
{
  char Z_s1[100], Z_s2[100];
  FILE *Z_f;
  strcpy( Z_s1, PV_file );
  strcat( Z_s1, "/record/" ); 
  Z_change( Z_k , Z_s2 );
  strcat( Z_s1, Z_s2 );
  Z_f = fopen( Z_s1, "r" );
  fscanf( Z_f, "%s", PV_now[Z_k] );
  fclose( Z_f );
}

void Z_f_writetime( long Z_k, char Z_s[] )
{
  char Z_s1[100], Z_s2[100];
  FILE *Z_f;
  strcpy( Z_s1, PV_file );
  strcat( Z_s1, "/record/" );
  Z_change( Z_k , Z_s2 );
  strcat( Z_s1, Z_s2 );
  Z_f = fopen( Z_s1, "w" );
  fprintf( Z_f, "%s\n", Z_s );
  fclose( Z_f );
}
  
double P_earned(char time[],int mode)
{
  char file[4][200];
  int year,month,day;
  int ly,lm,ld,b=0;
  int z,zz,zzz,zzzz,i;
  double a=0;
  char now0[100],day0[100],month0[100],year0[100];
  char Z_s[3][3] = {"e","i","o"};
  FILE *f0,*f1,*f2,*f3;
  
  int Z_i;
  for (Z_i=0;Z_i<=3;++Z_i) strcpy(file[Z_i],PV_file);

  i=mode;
  Z_f_readtime( mode );
  L_cuttime(time,&year,&month,&day,&z,&zz);

if (strcmp(time,PV_shijian)>0) Z_c_return(18);
if (year < PV_syear) Z_c_return(11);

  L_cuttime(PV_now[i],&ly,&lm,&ld,&zzz,&zzzz);
  if (mode!=0 && mode!=1 && mode!=2) return -12;
  
  strcpy(now0, strcat( strcat(file[0], "/record/now") , Z_s[mode]));
  strcpy(day0,strcat( strcat(file[1], "/record/day") , Z_s[mode]));
  strcpy(month0,strcat( strcat(file[2], "/record/month") , Z_s[mode]));
  strcpy(year0,strcat( strcat(file[3],"/record/year") , Z_s[mode]));

  for (Z_i=8;Z_i<=11;++Z_i) time[Z_i] = '0';

  if (day==ld && month==lm && year==ly)
  {
    f0=fopen(now0,"r"); fscanf(f0,"%lf",&a); fclose(f0);
    return a;
  }
  if (day!=ld && month==lm && year==ly)
  {
    f1=fopen(day0,"r");
    while(!feof(f1)) { fscanf(f1,"%lf %d",&a,&b); if (b==day){fclose(f1); return a;}}
    fclose(f1); return 0;
  }
  time[ 6 ] = '0';
  time[ 7 ] = '0';
  if (month!=lm && year==ly)
  {
    f2=fopen(month0,"r");
    while(!feof(f2)) { fscanf(f2,"%lf %d",&a,&b); if(b==month) {fclose(f2);return a;}}
    fclose(f2); return 0;
  }
  time[ 5 ] = '0';
  time[ 4 ] = '0';
  if (year!=ly)
  {
    f3=fopen(year0,"r");
    while(!feof(f3)) { fscanf(f3,"%lf %d",&a,&b); if(b==year) {fclose(f3);return a; }}
    fclose(f3); return 0;
  }
}

int P_earn(char time[],double m, int mode)
{
  if(mode!=0 && mode!=1 && mode!=2) return -1;

  char file[4][200];
  int year,month,day,i;
  int ly,lm,ld,tempint;
  double dsum=0,msum=0,ysum=0,x=0;
  int y = 0;
  int z,zz,zzz,zzzz;
  double a=0;
  char Z_s[3][3] = {"e","i","o"};
  char now0[100],day0[100],month0[100],year0[100]; 
  FILE *f0,*f1,*f2,*f3;

  int Z_i;
  for (Z_i=0;Z_i<=3;++Z_i) strcpy(file[Z_i],PV_file); 
  i=mode;
  Z_f_readtime( mode );

if ((m == 0) && (strcmp(time, PV_now[mode])==0)) return 0;

  L_cuttime(time,&year,&month,&day,&z,&zz);
  L_cuttime(PV_now[i],&ly,&lm,&ld,&zzz,&zzzz);

  strcpy(now0,strcat( strcat(file[0], "/record/now"), Z_s[mode]));
  strcpy(day0,strcat( strcat(file[1], "/record/day"), Z_s[mode]));
  strcpy(month0,strcat( strcat(file[2], "/record/month"), Z_s[mode]));
  strcpy(year0,strcat( strcat(file[3],"/record/year"), Z_s[mode]));

  if (day==ld && month==lm && year==ly)
  {
    f0=fopen(now0,"r"); fscanf(f0,"%lf",&dsum); fclose(f0);
    dsum+=m;
    f0=fopen(now0,"w"); fprintf(f0,"%lf\n",dsum); fclose(f0);
  }
  if (day!=ld && month==lm && year==ly)
  {
    f0=fopen(now0,"r"); fscanf(f0,"%lf",&dsum); fclose(f0);
    f0=fopen(now0,"w"); fprintf(f0,"%lf\n",m); fclose(f0);
    f1=fopen(day0,"a"); fprintf(f1,"%lf %d\n",dsum,ld); fclose(f1);
  }
  if (month!=lm && year==ly)
  {
    f1=fopen(day0,"r");
    while(!feof(f1)) { fscanf(f1,"%lf %d",&a,&tempint); msum=msum+a; }
    fclose(f1);
    f0=fopen(now0,"r"); fscanf(f0,"%lf",&dsum); fclose(f0);
    msum=msum+dsum;
    f1=fopen(day0,"w"); fprintf(f1,"%lf %d\n",x,y); fclose(f1);
    f2=fopen(month0,"a"); fprintf(f2,"%lf %d\n",msum,lm); fclose(f2);
    
    f0=fopen(now0,"w"); fprintf(f0,"%lf\n",m); fclose(f0);
  }
  if(year!=ly)
  {
    f2=fopen(month0,"r");
    while(!feof(f2)) { fscanf(f2,"%lf %d",&a,&tempint); ysum=a + ysum; }
    fclose(f2);
    f1=fopen(day0,"r");
    while(!feof(f1)) { fscanf(f1,"%lf %d",&a,&tempint); msum=a+msum; }
    fclose(f1);
    f0=fopen(now0 ,"r"); fscanf(f0,"%lf",&dsum); fclose(f0);
    ysum=ysum+msum+dsum;
    f3=fopen(year0,"a"); fprintf(f3,"%lf %d\n",ysum,ly); fclose(f3);
    f1=fopen(day0,"w"); fprintf(f1,"%lf %d\n",x,y); fclose(f1);
    f2=fopen(month0,"w"); fprintf(f2,"%lf %d\n",x,y); fclose(f2);

    f0=fopen(now0,"w"); fprintf(f0,"%lf\n",m); fclose(f0);
  }
  Z_f_writetime( mode, PV_shijian );
  P_earn(time, 0, 0);
  P_earn(time, 0, 1);
  P_earn(time, 0, 2);
  return 0;
}

int P_earn_c( int Z_c[] )
{
  
  Z_c[0] = versiona;
  Z_c[1] = versionb;
  Z_c[2] = versionc;
}

