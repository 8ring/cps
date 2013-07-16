#define c_versiona 0
#define c_versionb 0
#define c_versionc 2

/*----------------------------------end of include----------------------------------*/
/*----------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
/*-----------------------------------public define----------------------------------*/

#define P_max_file_name 30
#define P_max_car_name 20
#define P_max_user_name 30
#define P_max_date 12

typedef char PD_car[P_max_car_name + 1];
typedef char PD_user[P_max_user_name + 1];
typedef char PD_date[P_max_date + 1];
typedef int PD_time;
typedef int PD_position;
typedef char PD_park[P_max_file_name + 1];
typedef char PD_file_name[P_max_file_name * 3 + 1];

/*---------------------------------end of public define-----------------------------*/
/*----------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
/*-----------------------------------public variable--------------------------------*/

char PV_shijian[13];            /*the newest system time*/
/* at the beginning, it will be setted as "000000000000" */
/* then it will be setted as PV_syear + "00000000" */

long PV_capacity;              /*max cars can be stored in Car Park System*/
long PV_x;                                              
long PV_y; 
long PV_z;

int PV_disstart[2];            /*discount start time 0:hour;1:minutes*/ 
int PV_disend[2];              /*discount end time 0:hour;1:minutes*/
                                                          
double PV_norprice;            /*normal time period price*/      
                                                           
double PV_disprice;            /*discount time period price*/    
                                                          
double PV_threshold;           /*one unit free gift cost one threshold*/ 
                                                                 
double PV_gift;                /*one unit free gift cost one threshold*/ 

PD_park PV_file;               /*file name;car park system name*/

int PV_free;                   /*if park less than free-time, will free*/

int PV_decimal;                /*use decimal to change real money to long integer, can be + or -*/

int PV_syear;                  /*the start year of cps*/

int PV_lastyear;               /*cps last so many years*/

char PV_now[3][13];            /*last operating time for record*/
/*0: money
  1: in flow
  2: out flow
*/

char PV_tempfi[91];            /*used to share file between functions*/

extern char *PV_mess[20];	/*Error message passed by functions*/

/*----------------------------end of public variable--------------------------------*/
/*----------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
/*-------------------------------public function------------------------------------*/

/*[[1]]build the cps,  1:capacity; 2:x; 3:y; 4:z; 5:name of cps*/
int P_build( long, long, long, long, PD_park );

/*[[2]]set up period discount, 
  1~2:PV_disstart; PV_distart[0]: hour; PV_distart[1]: minutes
  3~4:PV_disend; the same as PV_distart
  5:PV_norprice; 6:PV_disprice*/ 
int P_pdiset(int, int, int, int, double, double); 
/*-1 if invalid discount ; +1 success*/

/*[[3]]set up accumulation discount, 1:PV_threshold; 2:PV_gift */
int P_adiset(double, double);
/*-1 if gift is no less than threshold ; +1 success*/

/*[[4]]power shut safety*/
int P_power( PD_park );

/*[[5]]convert car park number into the location(x,y,z), 1:number; 2:x, 3:y; 4:z*/
int P_convert(long *, long *, long *, long *);

/*[[6]]search a car by position( car park number ), 1:car park number; return car ID*/
char *P_posscar( long, char *, char *);
/*1:car park number; 2: date; 3: user*/

/*[[7]]search a car by car ID*/
long P_carspos( char *, char  *, char *);
/*1:car ID; 2: date; 3: user*/

/*[[8]]return the empty room for cars*/
long P_space(void);

/*[[9]]input a car*/
/*1:car ID; 2: date; 3: user 4:the position car parked*/
int P_iinput(char [21], char [13], char [31], long *);

/*[[10]]debug for the input car*/
/*1:car ID; 2: date;*/
int P_indebug(char [21], char [13], char [31]);

/*[[11]]movement on the database to insert a car*/
/*1:car ID; 2: date; 3: user 4:the position car parked*/
int P_insert( char *, char *, char * , long *);

/*[[12]]output a car*/
/*1:car ID; 2: date(out); 3: user(out)*/
int P_oinput(char [21], char [13], char [31], double *);

/*[[13]]debug for car information*/
/*1:car ID; 2: date(out);*/
int P_outdebug(char [21], char [13], char [31]);

/*[[14]]movement on database to move out a car*/
/*1:car ID; 2: date(return checkin time); 3: user(return check in user)*/
int P_get(char *,char *,char *);

/*[[15]]readout car information from the harddisk*/
int P_readout( char *, long *, long *, double *, double *);
/*1:car name; 2:park time; 3:total last time; 4:total money 5:p return file name: use PV_tempfi*/ 

/*[[16]]calculate the money to pay*/
double P_money(char [13], char [13], double *);
/*1:in time; 2:out time; 3:changable p*/

/*[[17]]write the information into a file*/
int P_readin( char *, double *, double *, char *, char *, char *, char *);
/*1: car ID; 2:money cost; 3:p; 4:user in; 5:user out; 6:date in; 7:date out*/

/*[[18]]empty~~*/

/*[[19]]record information to harddisk*/
int P_earn( char [13], double, int );
/*1: the record date
  2: money 
  3. 0: record money
     1: record in flow
     2: record out flow
*/

/*[[20]]output the recoeded information*/
double P_earned( char [13], int );
/*1: search date
  2. 0: search record money
     1: search record in flow
     2: search record out flow
*/

/*[[21]]empty~~*/

/*[[22]]empty~~*/

/*[[23]]empty~~*/

/*[[24]]renew the corrent time*/
int P_time( char [13] );

/*[[25]]calculate how many minutes in that period of time*/
long P_gap( char [13], char [13]);
/*1:start time; 2:end time*/

/*[[26]]search car by user name interval*/
long P_userscar( char *, char * );
/*1: start user; 2:end user;  return file name: use PV_tempfi*/

/*[[27]]search car by park date interval*/
long P_datescar( char *, char * );
/*1: start date; 2:end date;  return file name: use PV_tempfi*/

/*[[28]]format the power saving file*/
int P_format( void );

/*[[29]]show the progress bar*/
int P_progress( char *, double );

/*[[30]] -- [[36]]used to retrieve version*/
int P_ui_c( int [] );
int P_earn_c( int [] );
int P_store_c( int [] );
int P_record_c( int [] );
int P_power_c( int [] );
int P_lxs_c( int [] );
int P_yhz_c( int [] );

int P_status(const char[]);	/* status bar */

/*---------------------------------end of public function---------------------------*/
/*----------------------------------------------------------------------------------*/

/*-------------------------------------------gy-------------------------------------*/
/*-------------------------------------------gy-------------------------------------*/


/*-------------------------------------------lxs------------------------------------*/

int L_cuttime(char [], int *, int *, int *, int *, int *);
double L_discount(char *,char *);
double L_gift(double *,double );
int L_year(int);

/*---------------------------------------end of lxs---------------------------------*/


/*-------------------------------------------yhz------------------------------------*/
/*-------------------------------------------yhz------------------------------------*/


/*-------------------------------------------zcz------------------------------------*/

/*the max number of cars*/
#define Z_max_capacity 1000000
#define Z_max_capacity_cs "1000000"
#define Z_modnum 9973
#define Z_maxline 100
#define Z_debug 0

/*for the 4 pointers:
  0: empty room
  1: car name
  2: user name
  3: date
*/
struct Z_car
{
  char car[21];
  char user[31];
  char date[13];
  long father[4], sons[4][2];
  long number, total[4];
};

extern struct Z_car *Z_cc;

PD_file_name Z_file_name_car_hash;
PD_file_name Z_file_name_share;
PD_file_name Z_file_name_power;
PD_file_name Z_file_name_record;

long Z_head[4];

long Z_true_false;
/*0:false; outside of P_power
  1:true:  inside of P_power
*/

void Z_build_file( void );

/*1:the type of job:
    0 stand for insert
    1 stand for get 
*/
void Z_add( long, char [21], char[13], char [31] );

/*store.c*/
long Z_insert( long , long );
long Z_compare(long , long , long );
void Z_rotate(long, long, long );
void Z_splay(long, long );
void Z_help_splay( long, long );
long Z_get_number(long, long);
long Z_find( long , long, char *, long );
long Z_delet( long );
void Z_record_date_user( long, FILE * );
void Z_string_plus( char [], char);

/*record.c*/
void Z_change( long , char * );
void Z_new_car_hash_file( long );
long Z_record_check_car( char *, long );
long Z_hash( char * );
void Z_transform_file( char *, char *, char * );
void Z_touch_build_file( char * );
int P_counter_file( char [], char [], long );

/*power.c*/

/*earn.c*/
void Z_f_readtime( long );
void Z_f_writetime( long , char [] );

/*lxs.c*/
double Z_double_double( double );
double Z_period( long , long );

#define Z_c_file(a,b) if (chdir(a)==-1) {P_status( PV_mess[b] );return -1;}chdir("..");
#define Z_c_return(k) {P_status(PV_mess[k]);return -k;}
/*-------------------------------------------zcz------------------------------------*/
