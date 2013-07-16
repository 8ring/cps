#include <string.h>
#include <stdio.h>
#include"core.h"

#define versiona 0
#define versionb 0
#define versionc 2

#define Z_refresh_time 10000

int P_format( void )
{
  PD_file_name Z_s1, Z_s2, Z_temp_s;
  FILE *Z_f;  
  long Z_flag, Z_num, Z_i, Z_o;

  strcpy( Z_s1, Z_file_name_power );
  strcat( Z_s1, "sign" );
  Z_f = fopen( Z_s1, "r" );
  fscanf( Z_f, "%ld", &Z_flag );
  fscanf( Z_f, "%ld", &Z_num );
  fclose( Z_f ); 
 
  Z_flag = (Z_flag + 1) % 2;

  Z_num = 0;
  if (Z_flag == 0) strcpy( Z_temp_s, "0/" );
  else strcpy( Z_temp_s, "1/" );
  
  strcpy( Z_s1, Z_file_name_power );
  strcat( Z_s1, Z_temp_s );

  strcpy( Z_s2, Z_s1 );
  strcat( Z_s2, "var" );
  Z_f = fopen( Z_s2, "w" );
  
  fprintf(Z_f, "%s\n", PV_shijian);
  fprintf(Z_f, "%ld\n", PV_capacity);
  fprintf(Z_f, "%ld\n", PV_x);
  fprintf(Z_f, "%ld\n", PV_y);
  fprintf(Z_f, "%ld\n", PV_z);
  fprintf(Z_f, "%d\n", PV_disstart[0] );
  fprintf(Z_f, "%d\n", PV_disstart[1] );
  fprintf(Z_f, "%d\n", PV_disend[0] );
  fprintf(Z_f, "%d\n", PV_disend[1] );
  fprintf(Z_f, "%lf\n", PV_norprice );
  fprintf(Z_f, "%lf\n", PV_disprice );
  fprintf(Z_f, "%lf\n", PV_threshold );
  fprintf(Z_f, "%lf\n", PV_gift );
  fprintf(Z_f, "%s\n", PV_file );
  fprintf(Z_f, "%d\n", PV_free );
  fprintf(Z_f, "%d\n", PV_decimal );
  fprintf(Z_f, "%d\n", PV_syear );
  fprintf(Z_f, "%d\n", PV_lastyear );
  fprintf(Z_f, "%s\n", "00" ); //PV_now0
  fprintf(Z_f, "%s\n", "00" ); //PV_now1
  fprintf(Z_f, "%s\n", "00" ); //PV_now2

  fprintf(Z_f, "%s\n", Z_file_name_car_hash );
  fprintf(Z_f, "%s\n", Z_file_name_share );
  fprintf(Z_f, "%s\n", Z_file_name_power );
  fprintf(Z_f, "%s\n", Z_file_name_record );
  fprintf(Z_f, "%ld\n", Z_head[0] );
  fprintf(Z_f, "%ld\n", Z_head[1] );
  fprintf(Z_f, "%ld\n", Z_head[2] );
  fprintf(Z_f, "%ld\n", Z_head[3] );

  fclose( Z_f );
  
  strcpy( Z_s2, Z_s1 );
  strcat( Z_s2, "car" );
  Z_f = fopen( Z_s2, "w" );
  
  for (Z_i=0;Z_i<=PV_capacity;++Z_i)
  {

double Z_xx=Z_i,Z_yy=PV_capacity;
P_progress("Formating...", Z_xx/Z_yy );

    fprintf( Z_f, "%ld\n", Z_cc[Z_i].number );
    for (Z_o=0;Z_o<=3;++Z_o)
    {
      fprintf(Z_f, "%ld\n", Z_cc[Z_i].father[Z_o] );
      fprintf(Z_f, "%ld\n", Z_cc[Z_i].sons[Z_o][0] );
      fprintf(Z_f, "%ld\n", Z_cc[Z_i].sons[Z_o][1] );
      fprintf(Z_f, "%ld\n", Z_cc[Z_i].total[Z_o] );
    }
    if ((Z_cc[Z_i].total[0] == 0)&&(Z_i!=0))
    {
      fprintf(Z_f, "%s\n", Z_cc[Z_i].car );
      fprintf(Z_f, "%s\n", Z_cc[Z_i].user );
      fprintf(Z_f, "%s\n", Z_cc[Z_i].date );
    }
  }
  
  fclose( Z_f );  
  
  strcpy( Z_s2, Z_s1 );
  strcat( Z_s2, "com" );
  Z_f = fopen( Z_s2, "w" );
  fclose( Z_f );
  
  strcpy( Z_s1, Z_file_name_power );
  strcat( Z_s1, "sign" );
  Z_f = fopen( Z_s1, "w" );

  fprintf( Z_f, "%ld\n", Z_flag );
  fprintf( Z_f, "%ld\n", Z_num );
  fclose( Z_f );

  Z_true_false = 0;  

  return 0;
}          

void Z_add( long Z_k, char Z_car[], char Z_date[], char Z_user[] )
{
  PD_file_name Z_s1, Z_s2, Z_temp_s;
  FILE *Z_f;
  long Z_flag, Z_num, Z_i, Z_o;

  strcpy( Z_s1, Z_file_name_power );
  strcat( Z_s1, "sign" );
  Z_f = fopen( Z_s1, "r" );

  fscanf( Z_f, "%ld", &Z_flag );

  fscanf( Z_f, "%ld", &Z_num );
  fclose( Z_f );
  
  ++Z_num;

  Z_f = fopen( Z_s1, "w" );
  fprintf( Z_f, "%ld\n", Z_flag );
  fprintf( Z_f, "%ld\n", Z_num );
  fclose( Z_f );

  if (Z_flag == 0) strcpy( Z_temp_s, "0/" );
  else strcpy( Z_temp_s, "1/" );
 
  strcpy( Z_s1, Z_file_name_power );
  strcat( Z_s1, Z_temp_s );

  strcpy( Z_s2, Z_s1 );
  strcat( Z_s2, "com" );
  Z_f = fopen( Z_s2, "a" );
  
  fprintf( Z_f, "%ld\n", Z_k );
  fprintf( Z_f, "%s\n", PV_shijian );
  fprintf( Z_f, "%s\n", Z_car );
  if (Z_k==0)
  { 
    fprintf( Z_f, "%s\n", Z_date );
    fprintf( Z_f, "%s\n", Z_user );
  }
  fprintf( Z_f, "%ld\n", Z_num );
  fclose( Z_f );
  if (Z_num == Z_refresh_time) P_format(); 
}

int P_power( char Z_s[] )
{
  PD_file_name Z_ss, Z_temp_s, Z_sign, Z_svar, Z_scar, Z_scom;
  FILE *Z_f;
  long Z_flag, Z_num, Z_i, Z_o;
  char Z_car[21];
  char Z_date[13];
  char Z_user[31];
  char Z_tg[100];

Z_c_file( Z_s, 1 );
  
  Z_true_false = 1;

  strcpy( Z_ss, Z_s );
  strcat( Z_ss, "/power/" );
  strcpy( Z_sign, Z_ss );
  strcat( Z_sign, "sign" );

  Z_f = fopen( Z_sign, "r" );

if (Z_f == 0) Z_c_return(19);

  fscanf( Z_f, "%ld", &Z_flag );
  fscanf( Z_f, "%ld", &Z_num );
  fclose( Z_f );

  if (Z_flag == 0) strcpy( Z_temp_s, "0/" );
  else strcpy( Z_temp_s, "1/" );

  strcat( Z_ss, Z_temp_s );
  strcpy( Z_svar, Z_ss );strcat( Z_svar, "var" );
  strcpy( Z_scar, Z_ss );strcat( Z_scar, "car" );
  strcpy( Z_scom, Z_ss );strcat( Z_scom, "com" );
 
  Z_f = fopen( Z_svar, "r" );

if (Z_f == 0) Z_c_return(19);

  fscanf(Z_f, "%s", &PV_shijian);
  fscanf(Z_f, "%ld", &PV_capacity);
  fscanf(Z_f, "%ld", &PV_x);
  fscanf(Z_f, "%ld", &PV_y);
  fscanf(Z_f, "%ld", &PV_z);
  fscanf(Z_f, "%d", &PV_disstart[0] );
  fscanf(Z_f, "%d", &PV_disstart[1] );
  fscanf(Z_f, "%d", &PV_disend[0] );
  fscanf(Z_f, "%d", &PV_disend[1] );
  fscanf(Z_f, "%lf", &PV_norprice );
  fscanf(Z_f, "%lf", &PV_disprice );
  fscanf(Z_f, "%lf", &PV_threshold );
  fscanf(Z_f, "%lf", &PV_gift );
  fscanf(Z_f, "%s", &PV_file );
  fscanf(Z_f, "%d", &PV_free );
  fscanf(Z_f, "%d", &PV_decimal );
  fscanf(Z_f, "%d", &PV_syear );
  fscanf(Z_f, "%d", &PV_lastyear );
  fscanf(Z_f, "%s", &PV_now[0] );
  fscanf(Z_f, "%s", &PV_now[1] );
  fscanf(Z_f, "%s", &PV_now[2] );

  fscanf(Z_f, "%s", &Z_file_name_car_hash );
  fscanf(Z_f, "%s", &Z_file_name_share );
  fscanf(Z_f, "%s", &Z_file_name_power );
  fscanf(Z_f, "%s", &Z_file_name_record );
  fscanf(Z_f, "%ld", &Z_head[0] );
  fscanf(Z_f, "%ld", &Z_head[1] );
  fscanf(Z_f, "%ld", &Z_head[2] );
  fscanf(Z_f, "%ld", &Z_head[3] );
  fclose( Z_f );
  
  Z_f = fopen( Z_scar, "r" );

if (Z_f == 0) Z_c_return(19);

  for (Z_i=0;Z_i<=PV_capacity;++Z_i)
  {

double Z_xx=Z_i,Z_yy=PV_capacity;
P_progress("Readin...", Z_xx/Z_yy );

    fscanf( Z_f, "%ld", &Z_cc[Z_i].number );

if ((Z_cc[Z_i].number != Z_i) && (Z_i != 0)) Z_c_return(19);

    for (Z_o=0;Z_o<=3;++Z_o)
    {
      fscanf(Z_f, "%ld", &Z_cc[Z_i].father[Z_o] );
      fscanf(Z_f, "%ld", &Z_cc[Z_i].sons[Z_o][0] );
      fscanf(Z_f, "%ld", &Z_cc[Z_i].sons[Z_o][1] );
      fscanf(Z_f, "%ld", &Z_cc[Z_i].total[Z_o] );
    }
    if ((Z_cc[Z_i].total[0] == 0)&&(Z_i!=0))
    {
      fscanf(Z_f, "%s", &Z_cc[Z_i].car );
//      fscanf(Z_f, "%s", &Z_cc[Z_i].user );
      fgetc( Z_f );
      fgets( Z_tg, 40, Z_f );
long Z_pp= strlen(Z_tg);  
      Z_tg[Z_pp-1] = '\0';
      strcpy( Z_cc[Z_i].user, Z_tg );
      fscanf(Z_f, "%s", &Z_cc[Z_i].date );
    }
  }
  fclose( Z_f );

  Z_f = fopen( Z_scom, "r" );

if (Z_f == 0) Z_c_return(19);

  for (Z_i=1;Z_i<=Z_num;++Z_i)
  {
double Z_xx=Z_i,Z_yy=Z_num;
P_progress("Re new...", Z_xx/Z_yy );
    long Z_k;
    fscanf( Z_f, "%ld", &Z_k );
    fscanf( Z_f, "%s", &PV_shijian );
    fscanf( Z_f, "%s", &Z_car );
    if (Z_k==0)
    {
      fscanf( Z_f, "%s", &Z_date );
//      fscanf( Z_f, "%s", &Z_user );
      fgetc( Z_f );
      fgets( Z_tg, Z_maxline, Z_f );
long Z_pp= strlen(Z_tg);
      Z_tg[Z_pp-1] = '\0';
      strcpy( Z_user, Z_tg );
    }
    fscanf( Z_f, "%ld", &Z_o );
    if (Z_o != Z_i) 
    {
      fclose( Z_f );
      Z_c_return(19);
    }
    long Z_temp_for_pos;
    if (Z_k == 0) P_insert( Z_car, Z_date, Z_user ,&Z_temp_for_pos);
    else P_get( Z_car, Z_date, Z_user );
  }
  fclose( Z_f );
  
  Z_true_false = 0;
  
  if (Z_num == Z_refresh_time) P_format;
  return 0;
}

int P_power_c( int Z_c[] )
{

  Z_c[0] = versiona;
  Z_c[1] = versionb;
  Z_c[2] = versionc;
}

