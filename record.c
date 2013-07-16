#include <string.h>
#include <stdio.h>
#include "core.h"

#define versiona 0
#define versionb 0
#define versionc 3

char Z_hash_index[] = "hash.index";
char Z_root_index[] = "root.index";
char Z_suffix_total[] = ".total";
char Z_suffix_detail[] = ".detail";

long Z_hash_base[21] =
{99013,7,953,401,2557,13,571,709,2677,2,99929,9907,547,9341,9967,2857,1979,2999,2003,3413,19};

#define Z_max_char_line 100000

int P_counter_file( char Z_s[], char Z_ss[], long Z_line )
{
  FILE *Z_f, *Z_ff;
  long Z_i, Z_num, Z_minus, Z_l;
  long Z_c;
  char Z_temp_s[Z_max_char_line];
  if ((Z_f = fopen( Z_s, "r" ))==0) return -1;
  while (!feof(Z_f)) fgetc( Z_f );
  if ((Z_ff = fopen( Z_ss, "w" ))==0) return -1;
  while ((Z_num = ftell(Z_f))!= 0)
  {
    Z_minus = 0;
    for (Z_l=0;Z_l<Z_line;++Z_l)
    {
      if (fseek( Z_f, -1, SEEK_CUR )==-1) break;
      if (fgetc(Z_f) == '\n')
      {
        ++Z_minus;
        fseek( Z_f, -2, SEEK_CUR );
      } else fseek( Z_f, -1, SEEK_CUR );
      while (((Z_c=fgetc(Z_f))!='\n'))
      {

        ++Z_minus; 
        if (fseek( Z_f, -2, SEEK_CUR )==-1) 
        {
          fclose( Z_f );
          if ((Z_f = fopen( Z_s, "r" ))==0) return -1;
          
          break;          
        }
      }
    }
    for (;Z_l>0;--Z_l)
    {
      fgets( Z_temp_s, Z_max_char_line, Z_f );
      fprintf(Z_ff, "%s", Z_temp_s );
    }
    fseek( Z_f, ((-1)*Z_minus), SEEK_CUR );
  }
  fclose( Z_f );
  fclose( Z_ff );
  return 0;
}

void Z_change( long Z_k, char Z_s[] )
{
  long Z_o;
  char Z_ss[100];
  Z_o = 0;
  while (Z_k!=0)
  {
    Z_ss[Z_o++] = Z_k % 10 + '0';
    Z_k = Z_k / 10;
  }
  for (Z_k=0;Z_k<Z_o;++Z_k) Z_s[Z_k] = Z_ss[Z_o-1-Z_k];
  if (Z_o == 0) Z_s[Z_o++]='0';
  Z_s[Z_o]='\0';
}
  
void Z_build_file( void )
{
  PD_file_name Z_s1, Z_s2, Z_temp;
  long Z_i;
  FILE *Z_f;  

  strcpy( Z_s2, PV_file );
  strcat( Z_s2, "/" );
  strcat( Z_s2, "power" );
  strcpy( Z_s1, "mkdir " ); strcat( Z_s1, Z_s2 ); system( Z_s1 );
  strcpy( Z_file_name_power, Z_s2 );
  strcat( Z_file_name_power, "/" );

  strcpy( Z_s2, Z_file_name_power );
  strcat( Z_s2, "sign" );
  Z_f = fopen( Z_s2, "w" ); fprintf( Z_f, "%ld\n%ld\n", 0, 0 ); fclose( Z_f );

  strcpy( Z_s2, Z_file_name_power );
  strcat( Z_s2, "0" ); strcpy( Z_s1, "mkdir " ); strcat( Z_s1, Z_s2 ); system( Z_s1 );
  strcpy( Z_temp, Z_s2 );
  
  strcpy( Z_s2, Z_temp );strcat(Z_s2,"/var");strcpy(Z_s1,"touch ");strcat(Z_s1,Z_s2);system(Z_s1);
  strcpy( Z_s2, Z_temp );strcat( Z_s2, "/car" );strcpy( Z_s1, "touch " );strcat( Z_s1, Z_s2 );system( Z_s1 );
  strcpy( Z_s2, Z_temp );strcat( Z_s2, "/com" );strcpy( Z_s1, "touch " );strcat( Z_s1, Z_s2 );system( Z_s1 );

  strcpy( Z_s2, Z_file_name_power );
  strcat( Z_s2, "1" );
  strcpy( Z_s1, "mkdir " );
  strcat( Z_s1, Z_s2 );
  system( Z_s1 );
  strcpy( Z_temp, Z_s2 );

  strcpy( Z_s2, Z_temp );strcat( Z_s2, "/var" );strcpy( Z_s1, "touch " );strcat( Z_s1, Z_s2 );system( Z_s1 );
  strcpy( Z_s2, Z_temp );strcat( Z_s2, "/car" );strcpy( Z_s1, "touch " );strcat( Z_s1, Z_s2 );system( Z_s1 );
  strcpy( Z_s2, Z_temp );strcat( Z_s2, "/com" );strcpy( Z_s1, "touch " );strcat( Z_s1, Z_s2 );system( Z_s1 );

  strcpy( Z_s2, PV_file );
  strcat( Z_s2, "/" );
  strcat( Z_s2, "share" );
  strcpy( Z_s1, "mkdir " );strcat( Z_s1, Z_s2 );system( Z_s1 );
  strcpy( Z_file_name_share, Z_s2 );
  strcat( Z_file_name_share, "/" );

  strcpy( Z_s2, PV_file );
  strcat( Z_s2, "/" );
  strcat( Z_s2, "record" );
  strcpy( Z_s1, "mkdir " );strcat( Z_s1, Z_s2 );system( Z_s1 );
  strcpy( Z_file_name_record, Z_s2 );
  strcat( Z_file_name_record, "/" );

  strcpy( Z_s1, Z_s2 );
  
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/nowe");Z_f=fopen(Z_s2,"w");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/daye");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/monthe");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f); 
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/yeare");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/nowi");Z_f=fopen(Z_s2,"w");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/dayi");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/monthi");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/yeari");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/nowo");Z_f=fopen(Z_s2,"w");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/dayo");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/montho");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/yearo");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"0.000000 0\n");fclose(Z_f);

  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/0");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"%s\n","000101010000");fclose(Z_f);  
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/1");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"%s\n","000101010000");fclose(Z_f);
  strcpy(Z_s2,Z_s1);strcat(Z_s2,"/2");Z_f=fopen(Z_s2,"w");fprintf(Z_f,"%s\n","000101010000");fclose(Z_f);

  strcpy( Z_s2, PV_file );
  strcat( Z_s2, "/" );
  strcat( Z_s2, "car_data" );
  strcpy( Z_s1, "mkdir " );strcat( Z_s1, Z_s2 );system( Z_s1 );
  strcat( Z_s2, "/" );
  strcpy( Z_file_name_car_hash, Z_s2 );
  
  strcat( Z_s2, Z_root_index );Z_f = fopen( Z_s2, "w" );fclose( Z_f );
  
  return;
}

void Z_new_car_hash_file( long Z_k )
{
  FILE *Z_f;
  PD_file_name Z_s1, Z_s2;
  long Z_temp;
  char Z_ttt[100], Z_comp[100];
  Z_change( Z_k, Z_comp );
  strcpy( Z_s1, Z_file_name_car_hash);
  strcat( Z_s1, Z_root_index );
  Z_f = fopen( Z_s1, "r" );
  while (fgets(Z_ttt, Z_maxline, Z_f)>0) 
  {
    Z_ttt[strlen(Z_ttt)-1] = '\0';

    if (strcmp(Z_ttt,Z_comp)==0) 
    {
      fclose( Z_f );
      return;
    }
  } 
  fclose( Z_f );
  Z_f = fopen( Z_s1, "a" );
  fprintf( Z_f, "%ld\n", Z_k );
  fclose( Z_f ); 

  Z_change(Z_k, Z_s1);
  strcpy( Z_s2, Z_file_name_car_hash );
  strcat( Z_s2, Z_s1 );
  strcpy( Z_s1, "mkdir " );
  strcat( Z_s1, Z_s2 );
  system(Z_s1);
  
  strcat( Z_s2, "/" );
  strcat( Z_s2, Z_hash_index );
  Z_f = fopen( Z_s2, "w" );
  fclose( Z_f );
}


void Z_transform_file( char *Z_car, char *Z_s1, char *Z_s2 )
{
  long Z_hash_number;
  Z_hash_number = Z_hash( Z_car );
  Z_change( Z_hash_number, Z_s1 );
  strcpy( Z_s2, Z_file_name_car_hash);
  strcat( Z_s2, Z_s1 );
  strcat( Z_s2, "/" );
  strcat( Z_s2, Z_car );
  strcpy( Z_s1, Z_s2 );
  strcat( Z_s1, Z_suffix_total );
  strcat( Z_s2, Z_suffix_detail );
  return;
}
  
/*
  Z_type: 
    0: search but not build new one
    1: search and if there is not any one, build a new one
  
  return :
    0: no file
    1: there is a file for this car
*/ 
long Z_record_check_car( char *Z_car, long Z_type )
{
  PD_file_name Z_s1, Z_s2, Z_temp;
  FILE *Z_f;
  long Z_hash_number, Z_flag;
  
  Z_hash_number = Z_hash( Z_car );
  Z_new_car_hash_file( Z_hash_number );
  Z_change( Z_hash_number, Z_s1 );
  strcpy( Z_s2, Z_file_name_car_hash);
  strcat( Z_s2, Z_s1 );
  strcat( Z_s2, "/" );
  strcat( Z_s2, Z_hash_index ); 
  
  Z_f = fopen( Z_s2, "r" );
  while (fgets(Z_temp, Z_maxline, Z_f)>0)
  {
    Z_temp[strlen(Z_temp)-1] = '\0';
    if (strcmp(Z_temp, Z_car)==0)
    {
      fclose( Z_f );
      return 1;
    }
  }
  fclose( Z_f );
  if (Z_type==0) return 0;
  
  Z_f = fopen( Z_s2, "a" );
  fprintf( Z_f, "%s\n", Z_car );
  fclose( Z_f );
  
  Z_transform_file( Z_car, Z_s1, Z_s2 );
  Z_f = fopen( Z_s1, "w" );
  fprintf( Z_f, "0\n0\n0\n0\n" );
  fclose( Z_f );
  
  Z_f = fopen( Z_s2, "w" );
  fclose( Z_f );
  return 0;
}
  
long Z_hash( char Z_s[] )
{
  long Z_l, Z_t;
  Z_l = strlen( Z_s );
  Z_t = 1;
  while (Z_l>=0)
  {
    Z_t = (Z_s[Z_l]+Z_hash_base[Z_l]) * Z_t;
    Z_l--;
    Z_t = Z_t % Z_modnum;
  }
  return Z_t;
}
      
int P_readin( char *Z_car, double *Z_money, double *Z_p, char *Z_user_in, char *Z_user_out, char *Z_date_in, char *Z_date_out)
{
  PD_file_name Z_s1, Z_s2;
  long Z_times;
  long  Z_total_time;
  double Z_total_money; 
  long Z_get;
  FILE *Z_f;

if (strlen(Z_car)==0) Z_c_return(7);
  
  Z_get = Z_record_check_car( Z_car, 1 ); 
  Z_transform_file( Z_car, Z_s1, Z_s2 );
    
  Z_f = fopen( Z_s1, "r" );
  fscanf( Z_f, "%ld", &Z_times );
  fscanf( Z_f, "%ld", &Z_total_time );
  fscanf( Z_f, "%lf", &Z_total_money );
  fclose( Z_f );
  
  ++Z_times;
  Z_total_time = Z_total_time + (P_gap( Z_date_in, Z_date_out ));
  Z_total_money = Z_total_money + *Z_money; 
  
  Z_f = fopen( Z_s1, "w" );
  fprintf( Z_f, "%ld\n", Z_times );
  fprintf( Z_f, "%ld\n", Z_total_time );
  fprintf( Z_f, "%lf\n", Z_total_money );
  fprintf( Z_f, "%lf\n", *Z_p );
  fclose( Z_f );

  Z_f = fopen( Z_s2, "a" );
  fprintf( Z_f, "%lf\n", *Z_money );
  fprintf( Z_f, "%s\n", Z_user_in );
  fprintf( Z_f, "%s\n", Z_user_out );
  fprintf( Z_f, "%s\n", Z_date_in );
  fprintf( Z_f, "%s\n", Z_date_out );
  fclose( Z_f );
 
  return 0;
}

int P_readout( char *Z_car, long *Z_times, long *Z_total_time, double *Z_total_money, double *Z_p )
{
  PD_file_name Z_s1, Z_s2, Z_s3;
  long Z_get;
  FILE *Z_f, *Z_ff, *Z_fff;
  long Z_i;
  PD_user Z_f_user_in, Z_f_user_out;
  PD_date Z_f_date_in, Z_f_date_out;
  double Z_f_money;
  char Z_tg[100];
  long Z_pp; 

if (strlen(Z_car)==0) Z_c_return(7);

  Z_get = Z_record_check_car( Z_car, 0 );
  Z_transform_file( Z_car, Z_s1, Z_s2 );
  if (Z_get==0) 
  {
    *Z_p = 0;
    *Z_times = 0;
    *Z_total_time = 0;
    *Z_total_money = 0;
    return 0;
  } 
  
  Z_f = fopen( Z_s1, "r" );
  fscanf( Z_f, "%ld", Z_times );
  fscanf( Z_f, "%ld", Z_total_time );
  fscanf( Z_f, "%lf", Z_total_money );
  fscanf( Z_f, "%lf", Z_p );
  fclose( Z_f );

  strcpy( Z_s3, PV_file );
  strcat( Z_s3, "/share/record" );
  strcpy( Z_s1, Z_s3 );
  strcat( Z_s1, "_temp" ); 
  Z_ff = fopen( Z_s1, "w" );
  char s[1000];

  Z_f = fopen( Z_s2, "r" );
  for (Z_i=1;Z_i<=*Z_times;++Z_i)
  { 
    fscanf( Z_f, "%lf", &Z_f_money );
    fgetc( Z_f );
//    fscanf( Z_f, "%s", &Z_f_user_in );
    fgets( Z_tg, 40, Z_f );
    Z_pp= strlen(Z_tg);
    Z_tg[Z_pp-1] = '\0';
    strcpy( Z_f_user_in, Z_tg );
//    fscanf( Z_f, "%s", &Z_f_user_out );
    fgets( Z_tg, 40, Z_f );
    Z_pp= strlen(Z_tg);
    Z_tg[Z_pp-1] = '\0';
    strcpy( Z_f_user_out, Z_tg );

    fscanf( Z_f, "%s", &Z_f_date_in );
    fscanf( Z_f, "%s", &Z_f_date_out );
    
    fprintf( Z_ff, "From: " );
    strcpy( s, Z_f_date_in );
    fprintf(Z_ff,"%c%c%c%c-%c%c-%c%cT%c%c:%c%cZ",s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8],s[9],s[10],s[11]);
    fprintf( Z_ff, "  %s\n", Z_f_user_in );    

    fprintf( Z_ff, "To:   " );
    strcpy( s, Z_f_date_out );
    fprintf( Z_ff, "%c%c%c%c-%c%c-%c%cT%c%c:%c%cZ", s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11] );
    fprintf( Z_ff, "  %s\n", Z_f_user_out );
    fprintf( Z_ff, "%.3lf\n", Z_f_money );
  }
  fclose( Z_f );
  fclose( Z_ff );
  P_counter_file( Z_s1, Z_s3, 3 );

  strcpy( PV_tempfi, Z_s3 );

  return (1);
}

int P_record_c( int Z_c[] )
{

  Z_c[0] = versiona;
  Z_c[1] = versionb;
  Z_c[2] = versionc;
}

