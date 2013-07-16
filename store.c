#include <string.h>
#include <stdio.h>
#include "core.h"

#define versiona 0
#define versionb 0
#define versionc 1

#define Z_build_unit 500000 

int P_convert( long *Z_num, long *Z_a, long *Z_b, long *Z_c )
{
  if (*Z_num==0) 
  {
    if (((*Z_a)>PV_x)||((*Z_a)<=0)) return -2;
    if (((*Z_b)>PV_y)||((*Z_b)<=0)) return -3;
    if (((*Z_c)>PV_z)||((*Z_c)<=0)) return -4;
    long Z_p = 0;
    Z_p += ((*Z_a)-1)*((PV_y)*(PV_z));
    Z_p += ((*Z_b)-1)*(PV_z);
    Z_p += (*Z_c);
    (*Z_num) = Z_p;
    if ((*Z_num) > PV_capacity) return -5;
    return 0;
  } else 
  {
    *Z_a = 0; *Z_b = 0; *Z_c = 0;
    if (((*Z_num)<=0)||((*Z_num)>PV_capacity)) return -1;
    long Z_p = (*Z_num), Z_i, Z_o;
    Z_i = Z_p / (PV_y*PV_z);
    Z_p = Z_p % (PV_y*PV_z);
    (*Z_a) = Z_i + 1;
    if (Z_p == 0)
    { Z_p = (PV_y) * (PV_z); --(*Z_a);}
    Z_i = Z_p / (PV_z);
    Z_p = Z_p % (PV_z);
    (*Z_b) = Z_i + 1;
    if (Z_p == 0)
    {
      Z_p = (PV_z);
      --(*Z_b);
    }
    (*Z_c) = Z_p;
    if (((*Z_a)>PV_x)||((*Z_a)<=0)) return -5;
    if (((*Z_b)>PV_y)||((*Z_b)<=0)) return -5;
    if (((*Z_c)>PV_z)||((*Z_c)<=0)) return -5;
    return 0;
  }
}

long Z_get_number(long Z_a, long Z_k )
{
  if (Z_a==0) return 0;
  return((Z_cc[Z_a]).total[Z_k]);
}

long Z_compare(long Z_a,long Z_b, long Z_k )
{
  if (Z_k==0)
  {
    if ((Z_cc[Z_a]).number>(Z_cc[Z_b]).number) return 1;
    if ((Z_cc[Z_a]).number<(Z_cc[Z_b]).number) return -1;
    return 0;
  }
  long Z_compr;
  if (Z_k==1) Z_compr = strcmp( (Z_cc[Z_a]).car, (Z_cc[Z_b]).car );
  if (Z_k==2) Z_compr = strcmp( (Z_cc[Z_a]).user, (Z_cc[Z_b]).user );
  if (Z_k==3) Z_compr = strcmp( (Z_cc[Z_a]).date, (Z_cc[Z_b]).date );
    
  if (Z_compr>0) Z_compr = 1;
  if (Z_compr<0) Z_compr = -1;
  if ((Z_compr==0)&&(Z_k!=1)) Z_compr = Z_compare( Z_a, Z_b, 1 ); 
  return Z_compr;
}

void Z_rotate(long Z_a, long Z_k, long Z_d )
{
  long Z_dd;
  long Z_b, Z_c, Z_e;

  Z_dd = (Z_d + 1) % 2;
  Z_b = (Z_cc[Z_a]).father[Z_k];
  
  if (Z_b == 0) return;
  Z_c = (Z_cc[Z_b]).father[Z_k];
  if (Z_c == 0) Z_head[Z_k] = Z_a;
  else
  {
    if ((Z_cc[Z_c]).sons[Z_k][0] == Z_b) (Z_cc[Z_c]).sons[Z_k][0] = Z_a;
    else (Z_cc[Z_c]).sons[Z_k][1] = Z_a;
  }
    
  (Z_cc[Z_b]).sons[Z_k][Z_d] = (Z_cc[Z_a]).sons[Z_k][Z_dd];
  (Z_cc[Z_b]).father[Z_k] = Z_a; 
  (Z_cc[Z_b]).total[Z_k] = Z_get_number((Z_cc[Z_b]).sons[Z_k][0], Z_k);
  (Z_cc[Z_b]).total[Z_k]+= Z_get_number((Z_cc[Z_b]).sons[Z_k][1], Z_k)+1;
  
  Z_e = (Z_cc[Z_a]).sons[Z_k][Z_dd];
  if (Z_e != 0) (Z_cc[Z_e]).father[Z_k] = Z_b;
  
  (Z_cc[Z_a]).father[Z_k] = Z_c;
  (Z_cc[Z_a]).sons[Z_k][Z_dd] = Z_b;
  (Z_cc[Z_a]).total[Z_k] =Z_get_number((Z_cc[Z_a]).sons[Z_k][0], Z_k);
  (Z_cc[Z_a]).total[Z_k]+=Z_get_number((Z_cc[Z_a]).sons[Z_k][1], Z_k)+1;
}
   
void Z_splay( long Z_a, long Z_k )
{
  long Z_b, Z_c;
  long Z_d;
  if (Z_a == 0) return;
  if ((Z_b = (Z_cc[Z_a]).father[Z_k]) == 0) return;
  while (Z_cc[Z_a].father[Z_k] != 0) Z_help_splay(Z_a, Z_k);
}
  
void Z_help_splay(long Z_a, long Z_k )
{
  long Z_b, Z_c;
  long Z_d;
  if (Z_a == 0) return;
  if ((Z_b = (Z_cc[Z_a]).father[Z_k]) == 0) return;
  if ((Z_c = (Z_cc[Z_b]).father[Z_k]) == 0) 
  {
    if ((Z_cc[Z_b]).sons[Z_k][0] == Z_a) Z_d = 0; else Z_d = 1;
    Z_rotate( Z_a, Z_k, Z_d );
    return;
  } else
  { 
    if ((Z_cc[Z_c]).sons[Z_k][0] == Z_b)
    {
      if ((Z_cc[Z_b]).sons[Z_k][0] == Z_a) 
      {
        Z_rotate( Z_b, Z_k, 0 );
        Z_rotate( Z_a, Z_k, 0 );
      } else 
      {
        Z_rotate( Z_a, Z_k, 1 );
        Z_rotate( Z_a, Z_k, 0 );
      }
    } else 
    {  
      if ((Z_cc[Z_b]).sons[Z_k][0] == Z_a)
      {  
        Z_rotate( Z_a, Z_k, 0 );
        Z_rotate( Z_a, Z_k, 1 );
      } else 
      {
        Z_rotate( Z_b, Z_k, 1 );
        Z_rotate( Z_a, Z_k, 1 );
      }
    }
  }
}

long Z_insert( long Z_a, long Z_k)
{ 
  Z_cc[Z_a].father[Z_k] = 0;
  Z_cc[Z_a].sons[Z_k][0] = 0;
  Z_cc[Z_a].sons[Z_k][1] = 0;
  Z_cc[Z_a].total[Z_k] = 1;

  if (Z_head[Z_k]==0)
  {
    Z_head[Z_k] = Z_a;
    return 0;
  } else
  {
    long Z_p, Z_q;
    long Z_tt;
    Z_q = 0;
    Z_p = Z_head[Z_k];
    while (Z_p!=0)
    {
      Z_tt = Z_compare(Z_a, Z_p, Z_k); 
      Z_q = Z_p;
      Z_p = (Z_tt<0) ? (Z_cc[Z_p].sons[Z_k][0]) : (Z_cc[Z_p].sons[Z_k][1]);
    }
    if (Z_tt<0) Z_cc[Z_q].sons[Z_k][0] = Z_a;
      else Z_cc[Z_q].sons[Z_k][1] = Z_a;
    Z_cc[Z_a].father[Z_k] = Z_q;
    while (Z_q != 0) 
    {
      ++(Z_cc[Z_q].total[Z_k]);
      Z_q = (Z_cc[Z_q]).father[Z_k];
    }

    Z_splay( Z_a, Z_k );
    return 0;
  }
}  
    
int P_build( long n, long a, long b, long c, char s[] )
{
  long Z_i, Z_o, Z_p;
  long Z_length; 
  char sss[100];

if (chdir(s)==0) 
{
  chdir("..");
  Z_c_return(2);
}
FILE *Z_c_f;
if ((Z_c_f=fopen(s,"r"))!=0) 
{
  fclose(Z_c_f);
  Z_c_return(2);
}

if ((n<=0)||(n>Z_max_capacity)) Z_c_return(3);
if ((a<=0)||(a>n)) Z_c_return(3);
if ((b<=0)||(b>n)) Z_c_return(3);
if ((c<=0)||(c>n)) Z_c_return(3);

  strcpy(PV_file, s);
  strcpy( sss, "mkdir " );
  strcat(sss, PV_file );
  system( sss );
  
  PV_capacity = n;
  PV_x = a;
  PV_y = b;
  PV_z = c;

  for (Z_i=0;Z_i<4;++Z_i) Z_head[Z_i] = 0;

  Z_cc[0].total[0] = 1;  

  Z_p = 0;
  for (Z_i=0;Z_i<=(Z_build_unit - 1);++Z_i)
  {
    Z_o = Z_i;
    if (Z_o==0) Z_o = Z_build_unit;
    while (Z_o<=PV_capacity)
    {
      ++Z_p;

double Z_xx=Z_p,Z_yy=PV_capacity;
P_progress("Building...", Z_xx/Z_yy );
 
      (Z_cc[Z_o]).number = Z_o;
      Z_insert( Z_o, 0 );
      Z_o = Z_o + Z_build_unit;
    }
  }
  Z_build_file();  

  return 0;  
}

long Z_find( long Z_k, long Z_d, char *Z_char, long Z_number )
{
  if (Z_char != 0) 
  {
    if (Z_k==1) strcpy(Z_cc[0].car, Z_char);
    if (Z_k==2) strcpy(Z_cc[0].user, Z_char);
    if (Z_k==3) strcpy(Z_cc[0].date, Z_char);
  }
  if (Z_number>0) Z_cc[0].number = Z_number;

  if (Z_d==0)
  {
    long Z_p, Z_q;
    long Z_tt;
    Z_q = 0;
    if ((Z_p = Z_head[Z_k])==0) return -1;
    while (Z_p != 0)
    {
      Z_tt = Z_compare( Z_p, 0, Z_k );
      if (Z_tt==0)
      {
        Z_splay( Z_p, Z_k );
        return 0;
      }
      Z_q = Z_p;
      Z_p = (Z_tt>0) ? (Z_cc[Z_p]).sons[Z_k][0] : (Z_cc[Z_p]).sons[Z_k][1];
    }
    return -1;
  }
  if ((Z_d == 2)||(Z_d == -2))
  {
    long Z_p, Z_q;
    long Z_tt;
    Z_q = 0;
    if ((Z_p = Z_head[Z_k])==0) return -1;
    while (Z_p != 0)
    {
      Z_q = Z_p;
      Z_p = (Z_d == 2) ? (Z_cc[Z_p]).sons[Z_k][1] : (Z_cc[Z_p]).sons[Z_k][0];
    }
    Z_splay( Z_q, Z_k );
    return 0;
  }
  if ((Z_d==1)||(Z_d==-1))
  {
    if (Z_number == 0) return -1;
    long Z_p, Z_q, Z_dd;
    Z_dd = (Z_d==1) ? 1 : 0;
    if (Z_number <0) 
    {
      Z_p = Z_head[Z_k]; 
      if (Z_p==0) return 0;
    }   
    else Z_p = Z_number;
    Z_q = Z_find( Z_k, Z_d, 0, ((Z_cc[Z_p].sons[Z_k][Z_dd])) );
    if (Z_q==0) return 0;
    strcpy( Z_cc[0].car, Z_cc[Z_p].car );
    if (Z_compare(0,Z_p,Z_k)==Z_d) return (Z_q); else Z_q = Z_p; 
    Z_dd = (Z_dd+1)%2;
    Z_dd = Z_find( Z_k, Z_d, 0, ((Z_cc[Z_p].sons[Z_k][Z_dd])) );
    if (Z_dd>0) return(Z_dd); else return(Z_q);
  } 
}

long Z_delet( long Z_k )
{
  long Z_p, Z_q;
  if ((Z_p = Z_head[Z_k])==0) return 0;
  if ((Z_cc[Z_p]).total[Z_k] == 1) 
  {
    Z_head[Z_k] = 0;
    return( Z_p );
  }
  if ((Z_cc[Z_p]).sons[Z_k][0] == 0)
  {
    Z_q = (Z_cc[Z_p]).sons[Z_k][1];
    Z_head[Z_k] = Z_q;
    (Z_cc[Z_q]).father[Z_k] = 0;
    return( Z_p );
  } else 
  {
    Z_q = (Z_cc[Z_p]).sons[Z_k][0];
    (Z_cc[Z_q]).father[Z_k] = 0;
    Z_head[Z_k] = Z_q;
    long Z_t;
    Z_t = Z_find( Z_k, 2, 0, 0 );
    Z_q = Z_head[Z_k];
    (Z_cc[Z_q]).sons[Z_k][1] = (Z_cc[Z_p]).sons[Z_k][1];
    (Z_cc[Z_q]).total[Z_k] = (Z_cc[Z_p]).total[Z_k] - 1;
    (Z_cc[((Z_cc[Z_p]).sons[Z_k][1])]).father[Z_k] = Z_q;
    return (Z_p);
  }
}
     
int P_insert( char *Z_car_name, char *Z_date, char *Z_user_name, long *Z_pospos )
{
  long Z_flag;
  long Z_temp;
  Z_flag = Z_find( 0, -2, 0, 0 );
  if (Z_flag==-1) return -1;
  Z_temp = Z_delet( 0 );
  strcpy( (Z_cc[Z_temp]).car , Z_car_name);
  strcpy( (Z_cc[Z_temp]).date , Z_date);
  strcpy( (Z_cc[Z_temp]).user , Z_user_name);
  Z_cc[Z_temp].total[0] = 0;
  Z_insert( Z_temp, 1 );
  Z_insert( Z_temp, 2 );
  Z_insert( Z_temp, 3 );

  if (Z_true_false == 0) Z_add( 0, Z_car_name, Z_date, Z_user_name );
  *Z_pospos = Z_temp;
  return 0;
}
 
int P_get( char *Z_car_name, char *Z_date, char *Z_user_name )
{
  long Z_flag;
  long Z_temp;
  Z_flag = Z_find( 1, 0, Z_car_name, 0 );
  if (Z_flag==-1) return -1;
  Z_temp = Z_delet( 1 );
  
  Z_splay( Z_temp, 2 );
  Z_temp = Z_delet( 2 );
  Z_splay( Z_temp, 3 );
  Z_temp = Z_delet( 3 );

  strcpy( (Z_car_name), ((Z_cc[Z_temp]).car) );
  strcpy( (Z_date), ((Z_cc[Z_temp]).date) );
  strcpy( (Z_user_name), ((Z_cc[Z_temp]).user) );
  
  strcpy( ((Z_cc[Z_temp]).car), "" );
  strcpy( ((Z_cc[Z_temp]).date), "");
  strcpy( ((Z_cc[Z_temp]).user), ""); 

  Z_insert( Z_temp, 0 );
  
  if (Z_true_false == 0) Z_add( 1, Z_car_name, Z_date, Z_user_name );  
  return 0;
}

long P_carspos( char *Z_car_name, char *Z_date, char *Z_user_name )
{
  long Z_flag;
  Z_flag = Z_find( 1, 0, Z_car_name, 0);
  if (Z_flag == -1) Z_c_return(14);
  long Z_temp;
  Z_temp = Z_head[1]; 
  Z_splay( Z_temp, 2 ); Z_splay( Z_temp, 3 );
  strcpy( (Z_date), ((Z_cc[Z_temp]).date) );
  strcpy( (Z_user_name), ((Z_cc[Z_temp]).user) );
  return( (Z_cc[Z_temp]).number );
} 

char *P_posscar( long Z_pos, char *Z_date, char *Z_user )
{
  long Z_kk = Z_pos;
  if ((Z_pos<=0)||(Z_pos>PV_capacity)) return(0);
  long Z_k = 0;
  while (Z_cc[Z_pos].father[Z_k] != 0) Z_pos = Z_cc[Z_pos].father[Z_k];
  if ((Z_head[0] == Z_pos)&&(Z_head[0] != 0)) return 0;
  Z_splay( Z_kk, 1 );
  Z_splay( Z_kk, 2 );
  Z_splay( Z_kk, 3 );
  strcpy( (Z_date), ((Z_cc[Z_kk]).date) );
  strcpy( (Z_user), ((Z_cc[Z_kk]).user) );
 
  return (&(Z_cc[Z_kk].car[0]));
}

void Z_record_date_user( long Z_q, FILE *Z_f )
{
  char s[13];
  strcpy( s, Z_cc[Z_q].date );
  fprintf( Z_f, "License: %s\n", Z_cc[Z_q].car );
  fprintf( Z_f, "Check-in Time: %c%c%c%c-%c%c-%c%cT%c%c:%c%cZ\n", s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11] );
  fprintf( Z_f, "Checked in by: %s\n", Z_cc[Z_q].user );
}

void Z_string_plus( char Z_s[], char Z_c )
{
  long Z_p;
  Z_p = strlen( Z_s );
  Z_s[Z_p] = Z_c;
  Z_s[Z_p + 1] = '\0';
}
  
long P_userscar( char *Z_user_qq, char *Z_user_hh )
{
  long Z_q, Z_h, Z_i, Z_kk;
  char Z_ss[200];  
  FILE *Z_f;
  long Z_numnum;
  char Z_user_q[100], Z_user_h[100];

  strcpy( Z_user_q , Z_user_qq );
  strcpy( Z_user_h , Z_user_hh );

if (strlen(Z_user_h)<30) Z_string_plus( Z_user_h, 'z');

  Z_kk = strcmp( Z_user_q, Z_user_h );
  if (Z_kk>0) Z_c_return(16);

  Z_q = Z_find( 2, 1, Z_user_q, -1 );
  if (Z_q <= 0) Z_c_return(15);
  Z_h = Z_find( 2, -1, Z_user_h, -1 );
  if (Z_h <= 0) Z_c_return(15);
  
  strcpy( Z_ss, PV_file );
  strcat( Z_ss, "/share/userscar" );
  Z_f = fopen( Z_ss, "w" );
  Z_record_date_user( Z_q, Z_f );
  Z_numnum = 1;
  do
  {  
    for (Z_i=0;Z_i<=3;Z_i++) Z_splay( Z_q, Z_i );
    if ((Z_cc[Z_q].sons[2][1] != 0) && (Z_q!=Z_h))
    {
      Z_q = Z_cc[Z_q].sons[2][1];
      while (Z_cc[Z_q].sons[2][0]!=0) Z_q = Z_cc[Z_q].sons[2][0];
      Z_record_date_user( Z_q, Z_f );
      ++Z_numnum;
    }
  }while( Z_q != Z_h );
  fclose( Z_f );
  strcpy( PV_tempfi, Z_ss );
  return (Z_numnum*3);
}

long P_datescar( char *Z_date_qq, char *Z_date_hh )
{
  long Z_q, Z_h, Z_i, Z_kk;
  char Z_ss[200];
  FILE *Z_f;
  long Z_numnum;

  char Z_date_q[100], Z_date_h[100];

  strcpy( Z_date_q , Z_date_qq );
  strcpy( Z_date_h , Z_date_hh );

if (strlen(Z_date_h)<12) Z_string_plus( Z_date_h, 'z');

  Z_kk = strcmp( Z_date_q, Z_date_h );
  if (Z_kk>0) Z_c_return(16);

  Z_q = Z_find( 3, 1, Z_date_q, -1 );
  if (Z_q <= 0) Z_c_return(15);
  Z_h = Z_find( 3, -1, Z_date_h, -1 );
  if (Z_h <= 0) Z_c_return(15);

  strcpy( Z_ss, PV_file );
  strcat( Z_ss, "/share/datescar" );
  Z_f = fopen( Z_ss, "w" );
  Z_record_date_user( Z_q, Z_f );
  Z_numnum = 1;
  do
  {
    for (Z_i=0;Z_i<=3;Z_i++) Z_splay( Z_q, Z_i );
    if ((Z_cc[Z_q].sons[3][1] != 0) && (Z_q != Z_h))
    {
      Z_q = Z_cc[Z_q].sons[3][1];
      while (Z_cc[Z_q].sons[3][0]!=0) Z_q = Z_cc[Z_q].sons[3][0];
      Z_record_date_user( Z_q, Z_f );
      ++Z_numnum;
    }
  }while( Z_q != Z_h );
  fclose( Z_f );
  strcpy( PV_tempfi, Z_ss );
  return(Z_numnum*3);
}

long P_space(void)
{
  if (Z_head[0]==0) return 0;
  else return (Z_cc[Z_head[0]].total[0]);
}

int P_store_c( int Z_c[] )
{

  Z_c[0] = versiona;
  Z_c[1] = versionb;
  Z_c[2] = versionc;
}

