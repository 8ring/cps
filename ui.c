#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <ctype.h>

#include "core.h"
#include "ui.h"

#define versiona 0
#define versionb 0
#define versionc 2

#define G_MAX_DIM 600
#define G_max(a, b) (((a) > (b)) ? (a) : (b))
#define G_err(a, b, c) if((a) == (b)) return (c);
#define G_right (COLS/4 - 1)

#define G_pos items[mep.i][mep.a][mep.o].pos
#define G_val items[mep.i][mep.a][mep.o].value
#define G_typ items[mep.i][mep.a][mep.o].type
#define G_fun items[mep.i][mep.a][mep.o].f
#define G_ip(a, b, c) items[a][b][c].pos	/* enum { head, body, tail } */
#define G_iv(a, b, c) items[a][b][c].value
#define G_it(a, b, c) items[a][b][c].type
#define G_if(a, b, c) items[a][b][c].f
/* menu entry type */
enum typ {
	G_tme, G_tli, G_tna, G_tfi, G_tnu, G_tpr, G_tt4, G_tt7, G_tt6, G_tt1, G_tcl, G_tfu,
};

/* bunch of limits */
#define G_con_x 40
#define G_con_y 22
#define G_his_x 40
#define G_his_y 1000
#define G_res_x G_max(G_MAX_DIM, COLS*3)
#define G_res_y 8000
#define G_mai_x G_max(G_MAX_DIM, COLS*3)
#define G_mai_y 36
#define G_sta_x	G_max(G_MAX_DIM, COLS*3)

#define G_cont_y G_middle(1, LINES - 4, 1)
#define G_vdiv (COLS - G_right)

#define G_mess(a) {P_status(PV_mess[a]); return -1;}
#define G_maxm 13

int P_status(const char[]);	/* status bar */

int G_cursor(void);
int G_start(void);
int G_uinit(void);		/* initi ui */
void G_frame(void);
int G_middle(int l, int r, int len);	/* middle of [l, r] with len */
void G_gmenu(void);
void G_gcon(int, int);
void G_go(int);
int G_input(void);
int G_inputi(void);
int G_inputi4(char *, int, int, char *);
int G_showm(WINDOW *, char *);
int G_showt(WINDOW *, char *);
void G_resu(int, char *);	/* manipulate result pad ; title line & new
				   or move for {-1, 0, 1} */
int G_maxday(char *);

int G_key(int);			/* Print key bar */
int G_keyput(const char *, const char *, const char *, const char *, const char *);

int G_checkn(int);
int G_checkf(int);
int G_checkp(int);
int G_checkfp(int);
int G_typin(int (*f) (int), char *, int, char *, char[]);	/* Actual line input
								   handle */
int G_quit(void);
void G_cltem(void);
int G_iinput(void);
int G_oinput(void);
int G_carspos(void);
int G_posscar(void);
int G_retem(long *, FILE *);	/* Print temp file on G_res */
int G_userscar(void);
int G_datescar(void);
int G_earned(void);
int G_readout(void);
int G_power(void);
int G_format(void);
int G_setup(void);
int G_time(void);
int G_melast(void);		/* Get last entry index */
int G_ghis(int, int);
int G_whis(char *, char *, char *, int);
int G_para(void);
int G_readm(void);
int G_save(void);
int G_rever(WINDOW *, int);
void G_repcon(WINDOW * w);	/* Print PARAMETERS on window */
void G_cancelres(void);

struct {
	int nuke;
	int reg[G_MAX_DIM];
} G_eraser = {
	0, {
	}
};

void G_clean(void);
void G_regclean(int sy, unsigned int sx, unsigned ey);	/* [sy, ey] */
void G_cleaninit(void);

WINDOW *G_his, *G_con, *G_mai, *G_res, *G_sta;
short int G_act;		/* enum { mai, con, his } */
int G_dying = 0;
char G_dyingmessage[127] = "";
int G_stalen = 0;

unsigned int G_cols, G_lines;

#define G_tempal 30
/* tmps */
char G_tempa[G_maxm + 1][G_tempal + 1];
long G_tempnum[G_maxm + 1];

struct result {
	/* pos */
	signed int i;
	/* length */
	unsigned int l;
	int (*f) (void);
} res;

struct men {
	char *value;
	unsigned int pos:2;
	unsigned int type:4;
	int (*f) (void);
} items[8][5][16];

#define G_MLL 050
#define IND_LAST(i, a) ((i) + (a) * 8)
int mlasts[G_MLL];

struct mew {
	unsigned short i;
	unsigned short a;
	unsigned short o;
} mep;

struct him {
	unsigned int x;
	unsigned int y;
} sim;

/* Populates menu */
void
G_menu(void)
{
	items[0][0][0].value = "Parking Operations";	/* TOP menu */
	items[0][0][1].value = "Search";
	items[0][0][2].value = "Customer Records";
	items[0][0][3].value = "Business Records";
	items[0][0][4].value = "Parameters & Updating";
	items[0][0][5].value = "Read Me";
	items[0][0][6].value = "Quit";
	items[0][0][0].type = G_tme;
	items[0][0][1].type = G_tme;
	items[0][0][2].type = G_tme;
	items[0][0][3].type = G_tme;
	items[0][0][4].type = G_tme;
	items[0][0][5].type = G_tfu;
	items[0][0][6].type = G_tfu;
	items[0][0][0].pos = 0;
	items[0][0][1].pos = 1;
	items[0][0][2].pos = 1;
	items[0][0][3].pos = 1;
	items[0][0][4].pos = 1;
	items[0][0][5].pos = 1;
	items[0][0][6].pos = 2;
	G_if(0, 0, 6) = &G_quit;
	G_if(0, 0, 5) = &G_readm;

	items[1][0][0].value = "Check In";
	items[1][0][1].value = "Check Out";
	items[1][0][2].value = "Back";
	items[1][0][0].type = G_tme;
	items[1][0][1].type = G_tme;
	items[1][0][2].type = G_tme;
	items[1][0][0].pos = 0;
	items[1][0][1].pos = 1;
	items[1][0][2].pos = 2;


	items[1][1][0].value = "License #:";
	items[1][1][1].value = "Time:";
	items[1][1][2].value = "Customer:";
	items[1][1][3].value = "[ Clear ]";
	items[1][1][4].value = "[ Check In ]";
	items[1][1][5].value = "[ Back ]";
	items[1][1][0].type = G_tli;
	items[1][1][1].type = G_tt7;
	items[1][1][2].type = G_tna;
	items[1][1][3].type = G_tcl;
	items[1][1][4].type = G_tfu;
	items[1][1][5].type = G_tme;
	items[1][1][0].pos = 0;
	items[1][1][1].pos = 1;
	items[1][1][2].pos = 1;
	items[1][1][3].pos = 1;
	items[1][1][4].pos = 1;
	items[1][1][5].pos = 2;
	G_if(1, 1, 4) = &G_iinput;


	items[1][2][0].value = "License #:";
	items[1][2][1].value = "Time:";
	items[1][2][2].value = "Customer:";
	items[1][2][3].value = "[ Clear ]";
	items[1][2][4].value = "[ Check Out ]";
	items[1][2][5].value = "[ Back ]";
	items[1][2][0].type = G_tli;
	items[1][2][1].type = G_tt7;
	items[1][2][2].type = G_tna;
	items[1][2][3].type = G_tcl;
	items[1][2][4].type = G_tfu;
	items[1][2][5].type = G_tme;
	items[1][2][0].pos = 0;
	items[1][2][1].pos = 1;
	items[1][2][2].pos = 1;
	items[1][2][3].pos = 1;
	items[1][2][4].pos = 1;
	items[1][2][5].pos = 2;
	G_if(1, 2, 4) = &G_oinput;


	items[2][0][0].value = "By License #";
	items[2][0][1].value = "By position";
	items[2][0][2].value = "By Name";
	items[2][0][3].value = "By Check-in Time";
	items[2][0][4].value = "Back";
	items[2][0][0].type = G_tme;
	items[2][0][1].type = G_tme;
	items[2][0][2].type = G_tme;
	items[2][0][3].type = G_tme;
	items[2][0][4].type = G_tme;
	items[2][0][0].pos = 0;
	items[2][0][1].pos = 1;
	items[2][0][4].pos = 2;
	items[2][0][2].pos = 1;
	items[2][0][3].pos = 1;



	items[2][1][0].value = "License #:";
	items[2][1][1].value = "[ Clear ]";
	items[2][1][2].value = "[ Search ]";
	items[2][1][3].value = "[ Back ]";
	items[2][1][0].type = G_tli;
	items[2][1][1].type = G_tcl;
	items[2][1][2].type = G_tfu;
	items[2][1][3].type = G_tme;
	items[2][1][0].pos = 0;
	items[2][1][1].pos = 1;
	items[2][1][2].pos = 1;
	items[2][1][3].pos = 2;
	G_if(2, 1, 2) = &G_carspos;


	items[2][2][0].value = "Position #:";
	items[2][2][1].value = "X:";
	items[2][2][2].value = "Y:";
	items[2][2][3].value = "Z:";
	items[2][2][4].value = "[ Clear ]";
	items[2][2][5].value = "[ Search ]";
	items[2][2][6].value = "[ Back ]";
	items[2][2][0].type = G_tnu;
	items[2][2][1].type = G_tnu;
	items[2][2][2].type = G_tnu;
	items[2][2][3].type = G_tnu;
	items[2][2][4].type = G_tcl;
	items[2][2][5].type = G_tfu;
	items[2][2][6].type = G_tme;
	items[2][2][0].pos = 0;
	items[2][2][1].pos = 1;
	items[2][2][2].pos = 1;
	items[2][2][3].pos = 1;
	items[2][2][4].pos = 1;
	items[2][2][5].pos = 1;
	items[2][2][6].pos = 2;
	G_if(2, 2, 5) = &G_posscar;


	items[2][3][0].value = "From Name:";
	items[2][3][1].value = "To Name:";
	items[2][3][2].value = "[ Clear ]";
	items[2][3][3].value = "[ Search ]";
	items[2][3][4].value = "[ Back ]";
	items[2][3][0].type = G_tna;
	items[2][3][1].type = G_tna;
	items[2][3][2].type = G_tcl;
	items[2][3][3].type = G_tfu;
	items[2][3][4].type = G_tme;
	items[2][3][0].pos = 0;
	items[2][3][1].pos = 1;
	items[2][3][2].pos = 1;
	items[2][3][3].pos = 1;
	items[2][3][4].pos = 2;
	G_if(2, 3, 3) = &G_userscar;

	items[2][4][0].value = "From Time:";
	items[2][4][1].value = "To Time:";
	items[2][4][2].value = "[ Clear ]";
	items[2][4][3].value = "[ Search ]";
	items[2][4][4].value = "[ Back ]";
	items[2][4][0].type = G_tt7;
	items[2][4][1].type = G_tt7;
	items[2][4][2].type = G_tcl;
	items[2][4][3].type = G_tfu;
	items[2][4][4].type = G_tme;
	items[2][4][0].pos = 0;
	items[2][4][1].pos = 1;
	items[2][4][2].pos = 1;
	items[2][4][3].pos = 1;
	items[2][4][4].pos = 2;
	G_if(2, 4, 3) = &G_datescar;

	items[3][0][0].value = "License #:";
	items[3][0][1].value = "[ Clear ]";
	items[3][0][2].value = "[ Read ]";
	items[3][0][3].value = "[ Back ]";
	items[3][0][0].type = G_tli;
	items[3][0][1].type = G_tcl;
	items[3][0][2].type = G_tfu;
	items[3][0][3].type = G_tme;
	items[3][0][0].pos = 0;
	items[3][0][1].pos = 1;
	items[3][0][2].pos = 1;
	items[3][0][3].pos = 2;
	G_if(3, 0, 2) = &G_readout;


	items[4][0][0].value = "Business Volume";
	items[4][0][1].value = "Car flows";
	items[4][0][2].value = "Back";
	items[4][0][0].type = G_tme;
	items[4][0][1].type = G_tme;
	items[4][0][2].type = G_tme;
	items[4][0][0].pos = 0;
	items[4][0][1].pos = 1;
	items[4][0][2].pos = 2;

	items[4][1][0].value = "Date:";
	items[4][1][1].value = "[ Clear ]";
	items[4][1][2].value = "[ Read ]";
	items[4][1][3].value = "[ Back ]";
	items[4][1][0].type = G_tt6;
	items[4][1][1].type = G_tcl;
	items[4][1][2].type = G_tfu;
	items[4][1][3].type = G_tme;
	items[4][1][0].pos = 0;
	items[4][1][1].pos = 1;
	items[4][1][2].pos = 1;
	items[4][1][3].pos = 2;
	G_if(4, 1, 2) = &G_earned;

	items[4][2][0].value = "Date:";
	items[4][2][1].value = "[ Clear ]";
	items[4][2][2].value = "[ Read ]";
	items[4][2][3].value = "[ Back ]";
	items[4][2][0].type = G_tt6;
	items[4][2][1].type = G_tcl;
	items[4][2][2].type = G_tfu;
	items[4][2][3].type = G_tme;
	items[4][2][0].pos = 0;
	items[4][2][1].pos = 1;
	items[4][2][2].pos = 1;
	items[4][2][3].pos = 2;
	G_if(4, 2, 2) = &G_earned;

	items[5][0][0].value = "New Latest Time:";
	items[5][0][0].type = G_tt7;
	items[5][0][0].pos = 0;
	G_iv(5, 0, 1) = "[ Clear ]";
	G_iv(5, 0, 2) = "[ Update Time ]";
	G_ip(5, 0, 1) = 1;
	G_ip(5, 0, 2) = 1;
	G_it(5, 0, 1) = G_tcl;
	G_it(5, 0, 2) = G_tfu;
	G_if(5, 0, 2) = &G_time;
	items[5][0][3].value = "Show Parameters";
	items[5][0][3].type = G_tfu;
	G_if(5, 0, 3) = &G_para;
	items[5][0][3].pos = 1;
	items[5][0][4].value = "Backup Memory Info";
	items[5][0][4].type = G_tfu;
	G_if(5, 0, 4) = &G_format;
	items[5][0][4].pos = 1;
	items[5][0][5].value = "[ Back ]";
	items[5][0][5].type = G_tme;
	items[5][0][5].pos = 2;

	/* mep.i == 6->[readme] */
	G_iv(7, 0, 0) = "Continue Saved Session";
	/* ROOT menu */
	G_iv(7, 0, 1) = "Build A New Car Park";
	G_iv(7, 0, 2) = "Read me";
	G_iv(7, 0, 3) = "Quit";
	G_it(7, 0, 0) = G_tme;
	G_it(7, 0, 1) = G_tme;
	G_it(7, 0, 2) = G_tfu;
	G_it(7, 0, 3) = G_tfu;
	G_ip(7, 0, 0) = 0;
	G_ip(7, 0, 1) = 1;
	G_ip(7, 0, 2) = 1;
	G_ip(7, 0, 3) = 2;
	G_if(7, 0, 2) = &G_readm;
	G_if(7, 0, 3) = &G_quit;

	G_iv(7, 1, 0) = "Car Park Name:";
	G_iv(7, 1, 1) = "[ Clear ]";
	G_iv(7, 1, 2) = "[ Done ]";
	G_iv(7, 1, 3) = "[ Back ]";
	G_it(7, 1, 0) = G_tfi;
	G_it(7, 1, 1) = G_tcl;
	G_it(7, 1, 2) = G_tfu;
	G_it(7, 1, 3) = G_tme;
	G_ip(7, 1, 0) = 0;
	G_ip(7, 1, 1) = 1;
	G_ip(7, 1, 2) = 1;
	G_ip(7, 1, 3) = 2;
	G_if(7, 1, 2) = &G_power;


	items[7][2][0].value = "Car Park Name:";
	G_iv(7, 2, 1) = "Initial Year:";
	G_iv(7, 2, 2) = "End Year:";
	items[7][2][3].value = "X Capacity:";
	items[7][2][4].value = "Y Capacity:";
	items[7][2][5].value = "Z Capacity:";
	items[7][2][6].value = "Free Parking Time (min):";
	items[7][2][7].value = "Normal Price:";
	items[7][2][8].value = "Special Price:";
	items[7][2][9].value = "Special Period Begin:";
	items[7][2][10].value = "Special Period End:";
	items[7][2][11].value = "Cumulative Amount:";
	items[7][2][12].value = "Award Money:";
	G_iv(7, 2, 13) = "[ Clear ]";
	G_iv(7, 2, 14) = "[ Done ]";
	items[7][2][15].value = "[ Back ]";
	items[7][2][0].type = G_tfi;
	items[7][2][1].type = G_tt4;
	items[7][2][2].type = G_tt4;
	G_it(7, 2, 3) = G_tnu;
	G_it(7, 2, 4) = G_tnu;
	items[7][2][5].type = G_tnu;
	items[7][2][6].type = G_tnu;
	items[7][2][7].type = G_tpr;
	items[7][2][8].type = G_tpr;
	items[7][2][9].type = G_tt1;
	items[7][2][10].type = G_tt1;
	items[7][2][11].type = G_tpr;
	items[7][2][12].type = G_tpr;
	items[7][2][13].type = G_tcl;
	G_it(7, 2, 14) = G_tfu;
	G_it(7, 2, 15) = G_tme;
	items[7][2][0].pos = 0;;
	items[7][2][1].pos = 1;
	items[7][2][2].pos = 1;
	items[7][2][3].pos = 1;
	items[7][2][4].pos = 1;
	items[7][2][5].pos = 1;
	items[7][2][6].pos = 1;
	items[7][2][7].pos = 1;
	items[7][2][8].pos = 1;
	items[7][2][9].pos = 1;
	G_ip(7, 2, 10) = 1;
	G_ip(7, 2, 11) = 1;
	G_ip(7, 2, 12) = 1;
	G_ip(7, 2, 13) = 1;
	G_ip(7, 2, 14) = 1;
	G_ip(7, 2, 15) = 2;
	G_if(7, 2, 14) = &G_setup;

	int i;
	for (i = 0; i < G_MLL; i++)
		mlasts[i] = -1;
	for (i = 0; IND_LAST(6, i) < G_MLL; i++)
		mlasts[IND_LAST(6, i)] = 0;

}

/* my main */
void
G_larg(void)
{

	int i;

	G_lines = LINES;
	G_cols = COLS;

	sim.x = sim.y = 0;

	mep.i = 7;
	mep.a = 0;
	mep.o = 0;

	G_act = 0;
	res.i = 0;

	G_his = newpad(G_his_y, G_his_x);
	G_con = newpad(G_con_y, G_con_x);
	G_mai = newpad(G_mai_y, G_mai_x);
	G_res = newpad(G_res_y, G_res_x);
	G_sta = newpad(1, G_sta_x);

	if (G_his == 0 || G_con == 0 || G_mai == 0 || G_res == 0 || G_sta == 0) {
		G_dying |= 1;
		G_quit();
	}
	attrset(A_NORMAL);
	bkgdset((chtype) ' ');

	G_cltem();
	/* implicit G_cleaninit() */
	while (1) {
		if (is_term_resized(G_lines, G_cols)) {
			redrawwin(stdscr);
			G_regclean(-1, 0, 0);
		}
		G_clean();
		G_lines = LINES;
		G_cols = COLS;

		G_frame();
		if (mep.i != 6)
			G_gmenu();
		if (res.i == 1)
			G_resu(0, NULL);

		if (mep.i != 7 && (mep.i != 6 || mep.o != 7)) {
			G_gcon(0, 0);
			G_ghis(0, 0);
		}
		G_key(1);
		pnoutrefresh(G_sta, 0, 0, LINES - 1, 0, LINES - 1, COLS - 1);
		move(LINES - 3, 0);
		doupdate();
		G_cursor();
	}

}

int
G_start(void)
{

	G_err(initscr(), NULL, -1);

	G_err(cbreak(), ERR, -2);

	G_err(noecho(), ERR, -3);

	G_err(nonl(), ERR, -4);

	G_err(intrflush(stdscr, FALSE), ERR, -5);

	G_err(keypad(stdscr, TRUE), ERR, -6);

	return 0;

}

#define UINTF { "",	\
		"initscr",	\
		"cbreak",	\
		"noecho",	\
		"nonl",	\
		"intrflush",	\
		"keypad"	\
	}

int
G_uinit(void)
{

	int i;
	char *er[] = UINTF;

	if ((i = G_start()) < 0) {
		if (i != -1) {
			endwin();
		}
		sprintf(G_dyingmessage, "Errors occurred @ function %s while initializing GUI.\n", er[abs(i)]);
		G_dying |= 2;
		G_quit();
	}
	if (COLS >= 35 && LINES >= 10) {
		return 0;
	} else {
		sprintf(G_dyingmessage, "Screen too small. (Bypass with -f)\n");
		G_dying |= 1;
		return 1;
	}
}

void
G_frame(void)
{
	move(0, 0);
	attrset((G_act == 0) ? A_REVERSE : A_DIM);
	hline(' ', G_vdiv);
	move(0, G_middle(0, G_vdiv - 1, 11));
	addstr("MAIN WINDOW");
	attrset(A_NORMAL);
	move(0, G_vdiv + 1);
	attrset((G_act == 1) ? A_REVERSE : A_DIM);
	hline(' ', G_right);
	move(0, G_middle(G_vdiv + 1, COLS - 1, 9));
	addstr("PARAMETERS");
	move(0, G_middle(0, G_vdiv - 1, 11));
	attrset(A_NORMAL);
	move(G_cont_y, G_vdiv + 1);
	attrset((G_act == 2) ? A_REVERSE : A_DIM);
	hline(' ', G_right);
	move(G_cont_y, G_middle(G_vdiv + 1, COLS - 1, 10));
	addstr("LATEST OPS");
	attrset(A_NORMAL);
	move(0, G_vdiv);
	vline(ACS_VLINE, LINES - 3);

	wnoutrefresh(stdscr);
}

int
G_middle(int a, int b, int c)
{

	int i;

	i = b - a + 1;

	i = ((i - c) / 2) + a;

	return (i >= a) ? i : -1;

}

int
P_status(const char a[])
{
	werase(G_sta);

	if (a == NULL) {
		return 0;
	}
	int len = strlen(a) + 4;

	if (len > COLS || len > G_sta_x) {
		return -1;
	} else {
		wmove(G_sta, 0, G_middle(0, COLS - 1, len));
		wattrset(G_sta, A_REVERSE);
		waddstr(G_sta, "[ ");
		waddstr(G_sta, a);
		waddstr(G_sta, " ]");
		wattrset(G_sta, A_NORMAL);
		pnoutrefresh(G_sta, 0, 0, LINES - 1, 0, LINES - 1, COLS - 1);
	}

	return (COLS - G_stalen);

}

void
G_gmenu(void)
{
	int i = -1;

	wmove(G_mai, 0, 0);

	do {
		++i;
		wmove(G_mai, i * 2 + 1, 0);
		wclrtoeol(G_mai);
		if (mep.i != 7) {
			wattrset(G_mai, (i == mep.o) ? A_REVERSE : A_NORMAL);

			if (mep.i)
				wprintw(G_mai, "%d-", mep.i);
			if (mep.a)
				wprintw(G_mai, "%d-", mep.a);

			waddch(G_mai, '1' + i);
			wattrset(G_mai, A_NORMAL);
			waddstr(G_mai, ".");
		}
		waddstr(G_mai, " ");
		wattrset(G_mai, (i == mep.o) ? A_BOLD : A_NORMAL);
		waddstr(G_mai, G_iv(mep.i, mep.a, i));
		wattrset(G_mai, A_NORMAL);
		waddch(G_mai, ' ');
		if (G_it(mep.i, mep.a, i) >= G_tli && G_it(mep.i, mep.a, i) <= G_tt1) {

			switch (G_it(mep.i, mep.a, i)) {

			case G_tli:
			case G_tna:
			case G_tfi:
			case G_tt4:
				waddstr(G_mai, &G_tempa[i][0]);
				break;

			case G_tnu:
				if (strlen(&G_tempa[i][0]))
					wprintw(G_mai, "%d", G_tempnum[i]);
				break;

			case G_tpr:
				G_showm(G_mai, &G_tempa[i][0]);
				break;

			case G_tt7:
			case G_tt1:
			case G_tt6:
				G_showt(G_mai, &G_tempa[i][0]);
				break;

			}
			wattrset(G_mai, A_DIM);
			waddch(G_mai, ' ');
			wattrset(G_mai, A_NORMAL);
		}
	} while (items[mep.i][mep.a][i].pos < 2);

	int iy = LINES - 4, sy, uy = 2;

	sy = mep.o - iy / 2 + 1;

	if (sy < 0)
		sy = 0;

	if (iy <= 0) {
		return;
	} else if (iy == 1) {
		sy = sy * 2 - 1;
	} else {
		sy = sy * 2;
	}

	if (LINES - 4 > G_mai_y - sy) {
		uy = G_mai_y - sy;
		G_regclean(uy + 1, 0, LINES - 4);
	} else {
		uy = LINES - 4;
	}

	pnoutrefresh(G_mai, sy, 0, 1, 0, uy, G_vdiv - 1);

}

void
G_cancelres(void)
{
	res.i = 0;
	res.l = 0;
	P_status(NULL);
	if (mep.i == 6) {
		mep.i = mep.o;
		mep.o = 0;
	}
}

int
G_cursor(void)
{

	int i, j;

	j = getch();
	i = G_melast();

	switch (j) {

	case KEY_UP:
		if (G_act == 0) {
			if (res.i == 1)
				G_resu(-1, NULL);
			else {
				mep.o = (items[mep.i][mep.a][mep.o].pos == 0) ? i : (mep.o - 1);
			}
		} else if (G_act == 1)
			G_gcon(-1, 0);
		else if (G_act == 2)
			G_ghis(-1, 0);
		break;

	case KEY_DOWN:
		if (G_act == 0) {
			if (res.i == 1)
				G_resu(1, NULL);
			else
				mep.o = (items[mep.i][mep.a][mep.o].pos == 2) ? 0 : (mep.o + 1);
		} else if (G_act == 1)
			G_gcon(1, 0);
		else if (G_act == 2)
			G_ghis(1, 0);
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		if (mep.i == 7 || j - '1' > i || G_act != 0 || res.i != 0)
			break;
		else
			mep.o = j - '1';
		G_gmenu();
	case KEY_RIGHT:
		if (G_act == 0) {
			if (res.i == 1) {
				res.i = 2;
				res.l = 0;

				if (res.f == NULL) {
					res.i = 0;
				} else {
					(*res.f) ();
					res.i = 0;
					G_resu(2, NULL);
				}
			} else if (G_typ == G_tme) {
				if (G_pos == 2)
					G_go(-1);
				else
					G_go(1);
			} else if (G_typ == G_tfu)
				(*G_fun) ();
			else if (G_typ == G_tcl)
				G_cltem();
			else if (G_typ <= G_tpr)
				G_input();
			else
				G_inputi();
		} else if (G_act == 1)
			G_gcon(0, 1);
		else if (G_act == 2)
			G_ghis(0, 1);
		break;
	case KEY_LEFT:
		if (res.i == 1 && G_act == 0) {
			G_cancelres();
		} else if (G_act == 0 && (items[mep.i][mep.a][0].type == G_tme || items[mep.i][mep.a][0].type > G_tcl))
			G_go(-1);
		else if (G_act == 1)
			G_gcon(0, -1);
		else if (G_act == 2)
			G_ghis(0, -1);
		break;
	case '\t':
		if (mep.i == 7)
			break;
		++(G_act);
		(G_act) %= 3;
		break;

	}

}

/* cursor movement , up down ←→ */
void
G_go(int a)
{
	if (a < 0) {
		if (mep.i == 0 || (mep.i == 7 && mep.a == 0))
			/* at top level */
			return;
		if (mep.a == 0) {
			mep.o = mep.i - 1;
			mep.i = 0;
		} else {
			mep.o = mep.a - 1;
			mep.a = 0;
		}
	} else {
		if (mep.i == 0) {
			mep.i = mep.o + 1;
			mep.o = 0;
		} else {
			mep.a = mep.o + 1;
			mep.o = 0;
		}
	}

	P_status(NULL);
	G_cltem();
}

int
G_input(void)
{

	int i, max, (*f) (int);

	char *sta, v[G_tempal + 1];
	double m;

	switch (G_typ) {

	case G_tli:
		f = &isalnum;
		sta = "Allowed input: a-Z/0-9";
		max = P_max_car_name;

		break;

	case G_tna:
		sta = "Allowed input: a-Z/SPACE";
		f = &G_checkn;
		max = P_max_user_name;
		break;

	case G_tnu:
		f = &isdigit;
		sta = "Allowed input: 0-9";

		max = 7;
		break;

	case G_tfi:
		f = &G_checkf;
		sta = "Not allowed : spaces & slashes";
		max = P_max_file_name;
		break;

	case G_tpr:
		f = &G_checkp;
		sta = "Allowed input: 0-9/., accurate to .001";
		max = 6;
		break;

	}

	do {
		P_status(sta);
		i = G_typin(f, G_val, max, &G_tempa[mep.o][0], v);
		if (i < 0) {
			P_status(NULL);
			return 0;
		}
	} while ((G_typ == G_tpr && strchr(v, '.') != strrchr(v, '.')));

	P_status(NULL);

	strcpy(&G_tempa[mep.o][0], v);
	if (G_typ == G_tnu) {
		G_tempnum[mep.o] = atol(&G_tempa[mep.o][0]);
		sprintf(&G_tempa[mep.o][0], "%ld", G_tempnum[mep.o]);
	}
	if (G_typ == G_tpr) {

		m = strtod(v, NULL);
		sprintf(&G_tempa[mep.o][0], "%.3lf", m);

	}
	return 0;
}

int
G_inputi(void)
{

	int i;
	struct timeflag {
		unsigned int y:1;
		unsigned int m:1;
		unsigned int w:1;
	} tf;

	char yr[5];
	PD_date dd;

	switch (G_typ) {

	case G_tt7:
		tf.y = tf.m = tf.w = 1;
		break;

	case G_tt4:
		tf.y = 1;
		tf.m = tf.w = 0;
		break;

	case G_tt6:
		tf.y = tf.m = 1;
		tf.w = 0;
		break;

	case G_tt1:
		tf.y = tf.m = 0;
		tf.w = 1;
		break;
	}

	P_status(NULL);

	if (tf.y) {

		if ((i = G_inputi4(yr, 1, 9999, "Input Year:")) == 0) {

			move(LINES - 3, 0);
			clrtoeol();
			return 0;
		}
		int Z_i;
		char Z_s1[5];
		i = 4 - strlen(yr);
		strcpy(Z_s1, "");
		for (Z_i = 1; Z_i <= i; ++Z_i) {
			strcat(Z_s1, "0");
		}
		strcat(Z_s1, yr);
		strcpy(dd, Z_s1);
	}
	if (tf.m) {

		if ((i = G_inputi4(yr, 1, 12, "Input Month:")) == 0) {

			move(LINES - 3, 0);
			clrtoeol();
			return 0;
		}
		if (strlen(yr) == 1) {
			yr[1] = yr[0];
			yr[0] = '0';
			yr[2] = '\0';
		}
		strcat(dd, yr);

		if ((i = G_inputi4(yr, 1, G_maxday(dd), "Input Day:")) == 0) {

			move(LINES - 3, 0);
			clrtoeol();
			return 0;
		}
		if (strlen(yr) < 2) {
			yr[1] = yr[0];
			yr[0] = '0';
			yr[2] = '\0';
		}
		strcat(dd, yr);
	}
	if (tf.w) {

		if ((i = G_inputi4(yr, 0, 23, "Input Hour:")) == 0) {

			move(LINES - 3, 0);
			clrtoeol();
			return 0;
		}
		if (strlen(yr) < 2) {
			yr[1] = yr[0];
			yr[0] = '0';
			yr[2] = '\0';
		}
		if (tf.y)
			strcat(dd, yr);
		else
			strcpy(dd, yr);

		if ((i = G_inputi4(yr, 0, 59, "Input Minute:")) == 0) {

			move(LINES - 3, 0);
			clrtoeol();
			return 0;
		}
		if (strlen(yr) == 1) {
			yr[1] = yr[0];
			yr[0] = '0';
			yr[2] = '\0';
		}
		strcat(dd, yr);
	}
	strcpy(&G_tempa[mep.o][0], dd);

}
int
G_inputi4(char yr[], int ty, int mx, char *info)
{

	int i, max = 2;

	char a[5];

	if (mx == 9999)
		max = 4;

	P_status(NULL);

	while (1) {

		if (strlen(&G_tempa[mep.o][0]) != 0) {

			switch (info[6]) {

			case 'Y':
				strncpy(a, &G_tempa[mep.o][0], 4);
				a[4] = '\0';
				;

				break;
			case 'M':
				if (info[7] == 'o') {
					strncpy(a, &G_tempa[mep.o][4], 2);
					a[2] = '\0';
				} else {
					strncpy(a, &G_tempa[mep.o][10], 2);
					a[2] = '\0';
				}
				break;
			case 'D':
				strncpy(a, &G_tempa[mep.o][6], 2);
				a[2] = '\0';
				break;
			case 'H':
				strncpy(a, &G_tempa[mep.o][8], 2);
				a[2] = '\0';
				break;
			}
		} else
			a[0] = '\0';

		/* Nead */
		i = G_typin(isdigit, info, max, a, yr);

		if (i < 0)
			return 0;
		if (strlen(yr) > 0 && atoi(yr) >= ty && atoi(yr) <= mx)
			break;
	}
	if (G_typ == G_tt4)
		G_tempnum[mep.o] = atoi(yr);
	return 1;
}

int
G_maxday(char *a)
{

	int k, leap;
	k = atoi(a) / 100;

	if (k % 400 == 0 || (k % 4 == 0 && k % 100 != 0))
		leap = 1;
	else
		leap = 0;

	k = atoi(a) % 100;

	if ((k <= 7 && k % 2) || (k >= 8 && k % 2 == 0))
		return 31;
	else if (k == 2)
		return 28 + leap;
	else
		return 30;
}

int
G_checkn(int c)
{

	if (isalpha(c) || c == ' ')
		return 1;

	return 0;

}

int
G_checkf(int c)
{

	return (isprint(c) && !isspace(c) && c != '/' && c != '-');

}

int
G_checkp(int c)
{

	return (isdigit(c) || c == '.');

}

int
G_quit(void)
{
	if (!isendwin()) {
		endwin();
	}
/*
	WINDOW *ws[] = {G_his, G_con, G_mai, G_res, G_sta, NULL};
	WINDOW **p = ws;
	do {
		if (*p != NULL)
			free(*p);
	} while (*++p != NULL);
*/
	free(Z_cc);

	if (G_dyingmessage[0]) {
		fprintf(stderr, "\n%s", G_dyingmessage);
	}
	exit(G_dying);

}

int G_typin(int (*f) (int), char *title, int max, char *a, char v[]) {

	int i = 0, c, x, y, l;

	G_key(0);

	move(LINES - 3, 0);
	clrtoeol();

	printw("%s", title);

	refresh();

	getyx(stdscr, y, x);

	l = x;

	move(LINES - 3, l);
	addstr(a);
	i += strlen(a);
	(*f) (-2);

	while ((c = getch()) != KEY_DOWN) {

		switch (c) {

		case KEY_UP:

			move(LINES - 3, 0);
			clrtoeol();

			refresh();
			return -1;
			break;

		case KEY_RIGHT:

			getyx(stdscr, y, x);
			x += (x == i + l) ? 0 : 1;
			move(y, x);
			refresh();
			break;

		case KEY_LEFT:

			getyx(stdscr, y, x);
			x -= (x == l) ? 0 : 1;
			move(y, x);
			refresh();
			break;

		case 127:
		case KEY_BACKSPACE:
		case '\b':

			getyx(stdscr, y, x);
			if (x == l) {
				delch();
			} else {
				--x;
				move(y, x);
				delch();
				refresh();
			}
			if (--i < 0)
				i = 0;

			break;

		default:

			if ((*f) (c) && i + l - 1 <= COLS - 1 && i < max) {

				insch(c);
				getyx(stdscr, y, x);
				x += (i + l - 1 == COLS - 1) ? 0 : 1;
				move(y, x);
				refresh();
				++i;
			}
			break;
		}
	}

	move(LINES - 3, l);

	for (y = 0; y + 1 <= i; ++y) {

		x = inch() & A_CHARTEXT;
		if (G_typ == G_tli) {
			x = toupper(x);
		}
		v[y] = (char) x;
		move(LINES - 3, ++l);
	}

	v[y] = '\0';

	move(LINES - 3, 0);
	clrtoeol();

	return i;

}

void
G_cltem(void)
{
	int i;
	werase(G_mai);
	for (i = 0; i <= G_maxm; i++) {
		memcpy(&G_tempa[i][0], "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", G_tempal + 1);
		G_tempnum[i] = 0;
	}
}

void
G_resu(int yy, char *ti)
{

	int y;

	static int h, v;

	static char cti[G_MAX_DIM];

	if (yy > 1 || yy < -1) {
		werase(G_res);
		h = v = 0;
		if (ti != NULL) {
			strcpy(cti, ti);
			wmove(G_res, 0, 0);
			whline(G_res, ACS_HLINE, G_res_x);
			mvwaddstr(G_res, 0, G_middle(0, G_vdiv - 1, strlen(cti)), cti);
			mvwaddch(G_res, 0, G_vdiv, ACS_RTEE);
		}
		return;
	}
	wmove(G_res, 0, 0);
	whline(G_res, ACS_HLINE, G_res_x);
	mvwaddstr(G_res, 0, G_middle(0, G_vdiv - 1, strlen(cti)), cti);
	mvwaddch(G_res, 0, G_vdiv, ACS_RTEE);

	h += yy;

	y = G_melast();

	y = (mep.i == 6) ? 1 : y * 2 + 4;

	wmove(G_res, res.l + 1, 0);
	whline(G_res, '*', G_res_x);
	mvwprintw(G_res, res.l + 1, G_middle(0, G_vdiv - 1, 3), "EOF");

	if (h > res.l)
		h = res.l;
	if (h < 1)
		h = 1;

	if (y > LINES - 4) {
		G_cancelres();
		P_status("Screen too small.");
		return;
	}
	pnoutrefresh(G_res, 0, 0, y, 0, y, G_vdiv);

	pnoutrefresh(G_res, h, 0, y + 1, 0, LINES - 4, G_vdiv - 1);

}

int
G_oinput(void)
{

	long i, fei, fei2;
	char t[13], n[G_tempal + 1], b[G_tempal + 1], e[64] = "Check out Succeeded. Payment Bill:";
	double m, p;

	if (res.i == 2) {
		i = (long) P_oinput(&G_tempa[0][0], &G_tempa[1][0], &G_tempa[2][0], &m);
		sprintf(b, "%.3lf", m);
		G_whis(&G_tempa[0][0], PV_shijian, b, 1);
		res.i = 0;
		strcat(e, b);

		G_cltem();
		P_status(e);
		beep();

	} else {
		strcpy(t, PV_shijian);
		i = P_outdebug(&G_tempa[0][0], &G_tempa[1][0], &G_tempa[2][0]);
		strcpy(PV_shijian, t);
		t[0] = '\0';
		if (i >= 0) {
			i = P_carspos(&G_tempa[0][0], t, n);
			res.i = 1;
			res.f = &G_oinput;

			G_resu(2, "VEHICLE INFORMATION");
			mvwprintw(G_res, 1, 0, "Licence No.: ");
			waddstr(G_res, &G_tempa[0][0]);
			mvwprintw(G_res, 3, 0, "Check in Time: ");
			G_showt(G_res, t);
			mvwprintw(G_res, 5, 0, "Parking time: %ld min", P_gap(t, &G_tempa[1][0]));
			mvwprintw(G_res, 7, 0, "Checked in by: %s", n);
			mvwprintw(G_res, 9, 0, "Slot Position: %ld", i);
			P_readout(&G_tempa[0][0], &fei, &fei2, &m, &p);
			m = P_money(t, &G_tempa[1][0], &p);
			sprintf(b, "%.3lf", m);
			mvwaddstr(G_res, 11, 0, "Calculated Fee: ");
			G_showm(G_res, b);
			res.l = 11;
			P_status("Proceed to check out?");
		} else
			return -1;
	}
}

int
G_iinput(void)
{

	long i, k;
	char a[66] = "Check in Succeeded. Car position #", b[8], c[] = ". Remaining slots: ";

	if (P_space() <= 0) {
		P_status("No spare slot");
		return -1;
	}
	i = P_iinput(&G_tempa[0][0], &G_tempa[1][0], &G_tempa[2][0], &k);

	if (i >= 0) {

		sprintf(b, "%ld", k);
		G_whis(&G_tempa[0][0], &G_tempa[1][0], b, 0);
		strcat(a, b);
		i = P_space();
		sprintf(b, "%ld", i);
		strcat(a, c);
		strcat(a, b);
		P_status(a);
		G_cltem();
		beep();
	}
	return -1;
}

int
G_carspos(void)
{

	long i;
	char t[13], n[G_tempal + 1];

	i = P_carspos(&G_tempa[0][0], t, n);

	if (i >= 0) {
		res.i = 1;
		res.f = NULL;

		G_resu(2, "SEARCH RESULT");
		mvwprintw(G_res, 1, 0, "Licence No.: ");
		waddstr(G_res, &G_tempa[0][0]);
		mvwprintw(G_res, 3, 0, "Check in Time: ");
		G_showt(G_res, t);
		mvwprintw(G_res, 5, 0, "Checked in by: %s", n);
		res.l = 5;
	} else {
		return -1;
	}

}

int
G_posscar(void)
{

	long n = G_tempnum[0], m = 0, x = G_tempnum[3], y = G_tempnum[2], z = G_tempnum[1];
	char *l, t[13], s[13], c[G_tempal + 1], b[G_tempal + 1], *k;

	P_convert(&m, &x, &y, &z);

	l = P_posscar(n, t, c);
	k = P_posscar(m, s, b);

	if (l == NULL && k == NULL)
		P_status("Both not found");
	else {
		res.i = 1;
		res.f = NULL;
		res.l = 0;

		G_resu(2, "SEARCH RESULT");
		wattrset(G_res, A_UNDERLINE);
		mvwprintw(G_res, ++res.l, 0, "Searched by Position Number:");
		wattrset(G_res, A_NORMAL);
		if (l == NULL) {

			mvwprintw(G_res, 3, 1, "Not found.");
			res.l = 3;
		} else {

			mvwprintw(G_res, 3, 1, "License No.: %s", l);
			mvwprintw(G_res, 5, 1, "Check in Time: ");
			G_showt(G_res, t);
			mvwprintw(G_res, 7, 1, "Checked in by: %s", c);
			res.l = 7;
		}
		wattrset(G_res, A_UNDERLINE);
		mvwprintw(G_res, res.l + 2, 0, "Searched by Coordinates:");
		wattrset(G_res, A_NORMAL);
		if (k == NULL) {
			mvwprintw(G_res, res.l + 4, 1, "Not found.");
			res.l += 4;
		} else {
			mvwprintw(G_res, res.l + 4, 1, "License No.: %s", k);
			mvwprintw(G_res, res.l + 6, 1, "Check in Time: ");
			G_showt(G_res, s);
			mvwprintw(G_res, res.l + 8, 1, "Checked in by: %s", b);
			res.l += 8;
		}
	}
}

int
G_userscar(void)
{

	FILE *te;
	char de[100];
	long i;

	de[0] = '\0';

	i = P_userscar(&G_tempa[0][0], &G_tempa[1][0]);

	/* i : a multiple of 3 */
	if (i / 3 * 4 >= G_res_y)
		G_save();

	else if (i >= 0 && strlen(PV_tempfi) > 0) {
		te = fopen(PV_tempfi, "r");
		res.i = 1;

		res.f = &G_save;

		G_resu(2, "SEARCH RESULT");

		G_retem(&i, te);

		fclose(te);
		res.l = i;
	} else
		return -1;

}

int
G_datescar(void)
{

	long i;
	FILE *te;
	char d[91], de[100];
	d[0] = de[0] = '\0';

	i = P_datescar(&G_tempa[0][0], &G_tempa[1][0]);

	if (i / 3 * 4 >= G_res_y)
		G_save();

	else if (i >= 0 && strlen(PV_tempfi) > 0) {
		te = fopen(PV_tempfi, "r");
		res.i = 1;

		res.f = &G_save;
		G_resu(2, "SEARCH RESULT");

		G_retem(&i, te);

		fclose(te);
		res.l = i;
	} else
		return -1;

}

int
G_earned(void)
{
	/* Nead */
	double m, o;
	char sum[G_tempal + 1], mus[G_tempal + 1], dd[13];
	m = o = -1;
	strcpy(dd, &G_tempa[0][0]);

	if (mep.a == 1) {

		m = P_earned(dd, 0);

		if (m >= 0) {

			res.i = 1;
			res.l = 3;
			res.f = NULL;
			sprintf(sum, "%.3lf", m);
			G_resu(2, "INCOME RECORD");

			mvwaddstr(G_res, 1, 0, "Total Income: ");
			G_showm(G_res, sum);
			wattrset(G_res, A_DIM);
			wprintw(G_res, " ");
			wattrset(G_res, A_NORMAL);
			mvwaddch(G_res, 3, 1, '@');
			if (atoi(&dd[4]) == 0) {
				dd[4] = '\0';
				waddstr(G_res, dd);
			} else if (atoi(&dd[6]) == 0) {
				dd[6] = '\0';
				wprintw(G_res, "%.4s-%s", dd, &dd[4]);
			} else {
				dd[8] = '\0';
				G_showt(G_res, dd);
			}
			G_resu(0, NULL);

		} else
			return m;

	}
	if (mep.a == 2) {

		m = P_earned(dd, 1);
		/*
		if(atoi(&dd[4]) == 0)
			dd[5] = '1';
		if(atoi(&dd[6]) == 0)
			dd[7] = '1';
			*/
		o = P_earned(dd, 2);

		if (m >= 0 && o >= 0) {

			res.i = 1;
			res.f = NULL;
			G_resu(2, "VEHICLE FLOW RECORD");
			sprintf(sum, "%.0lf", m);
			sprintf(mus, "%.0lf", o);
			mvwprintw(G_res, 1, 0, "In Flow Record: %s", sum);
			mvwprintw(G_res, 3, 0, "Out Flow Record: %s", mus);
			mvwaddch(G_res, 5, 1, '@');
			if (atoi(&dd[4]) == 0) {
				dd[4] = '\0';
				waddstr(G_res, dd);
			} else if (atoi(&dd[6]) == 0) {
				dd[6] = '\0';
				wprintw(G_res, "%.4s-%s", dd, &dd[4]);
			} else {
				dd[8] = '\0';
				G_showt(G_res, dd);
			}
			res.l = 5;
			G_resu(0, NULL);
		} else
			return m;

	}
}

int
G_readout(void)
{

	long t, a;
	double m, p;
	int i;
	char e[61];
	FILE *te;

	i = P_readout(&G_tempa[0][0], &t, &a, &m, &p);
	/* Nead */

	if (i >= 0 && t > 0) {

		sprintf(e, "%.3lf", m);
		G_resu(2, "VEHICLE RECORD");
		mvwprintw(G_res, 1, 0, "Licence No.: %s", &G_tempa[0][0]);
		mvwprintw(G_res, 3, 0, "Viewed times: %ld", t);
		mvwprintw(G_res, 5, 0, "Total parking time: %ld min", a);
		mvwprintw(G_res, 7, 0, "Average parking time: %ld min", a / t);
		mvwprintw(G_res, 9, 0, "Total expenditure: ");
		G_showm(G_res, e);
		sprintf(e, "%.3lf", m / t);
		mvwprintw(G_res, 11, 0, "Average expenditure: ");
		G_showm(G_res, e);
		sprintf(e, "%.3lf", p);
		mvwprintw(G_res, 13, 0, "Cumulated amount: ");
		G_showm(G_res, e);
		mvwprintw(G_res, 15, 0, "Detailed Record:");

		if (16 + t * 4 >= G_res_y) {
			wprintw(G_res, "Please refer to files");
			res.l = 15;
			G_resu(0, NULL);
			G_save();
		} else {
			res.i = 1;
			res.f = &G_save;
			te = fopen(PV_tempfi, "r");
			for (t = 0, i = 0; fgets(e, 60, te) != NULL; ++t, ++i) {
				e[strlen(e) - 1] = '\0';
				if (t % 3 == 0)
					i++;
				mvwprintw(G_res, 16 + i, 0, "%s", e);
			}

			res.l = 15 + i;
			G_resu(0, NULL);
			fclose(te);
		}
	} else if (i != -7) {
		P_status("No record found");
		return -1;
	} else
		return -1;
}

int
G_retem(long *s, FILE * t)
{

	long i, j;
	char de[61];

	for (i = j = 1; i <= *s; i++, j++) {

		fgets(de, 60, t);
		de[strlen(de) - 1] = '\0';
		mvwprintw(G_res, j, 0, "%s", de);
		if (i % 3 == 0)
			j++;
	}
	*s = j - 2;
}

int
G_power(void)
{

	if (P_power(&G_tempa[0][0]) == 0) {
		mep.i = mep.o = mep.a = 0;
		refresh();
		flushinp();
		beep();
		P_status(NULL);
	}
}

int
G_setup(void)
{

	char t[13], **o, *po;
	int au = 0;

	po = t;
	o = &po;

#define ifautoset(a, b) if(!strlen(G_tempa[(a)])) {	\
	strncpy(G_tempa[(a)], (b), G_tempal);	\
	au = 1;	\
	}

	ifautoset(1, "0001")
		G_tempnum[1] = atoi(G_tempa[1]);

	if (!strlen(&G_tempa[2][0])) {
		G_tempnum[2] = 4000 + G_tempnum[1] - 1;
		if (G_tempnum[2] > 9999)
			G_tempnum[2] = 9999;
		sprintf(&G_tempa[2][0], "%ld", G_tempnum[2]);
		au = 1;
	} else {
		G_tempnum[2] = atoi(G_tempa[2]);
	}

	ifautoset(6, "0")	/* free time */
		G_tempnum[6] = atoi(G_tempa[6]);

	ifautoset(7, "0")	/* normal price */
		ifautoset(8, "0")	/* special period things */
		ifautoset(9, "0")
		ifautoset(10, "0")
		ifautoset(11, "9999.99")	/* cumulative thingies */
		ifautoset(12, "0")
		if (au) {
		P_status("Auto filling blanks ... done.");
		return -1;
	}
	if (G_tempnum[2] - G_tempnum[1] > 3999 || G_tempnum[2] - G_tempnum[1] <= 0) {
		G_mess(4);
		return -1;
	}
	if (P_pdiset(atoi(&G_tempa[9][0]) / 100, atoi(&G_tempa[9][0]) % 100, atoi(&G_tempa[10][0]) / 100, atoi(&G_tempa[10][0]) % 100, strtod(&G_tempa[7][0], o), strtod(&G_tempa[8][0], o)) < 0)
		return -1;
	if (P_adiset(strtod(&G_tempa[11][0], o), strtod(&G_tempa[12][0], o)) < 0)
		return -1;

	if (P_build(G_tempnum[3] * G_tempnum[4] * G_tempnum[5], G_tempnum[5], G_tempnum[4], G_tempnum[3], &G_tempa[0][0]) < 0) {
		return -1;
	}
	PV_free = G_tempnum[6];
	PV_lastyear = G_tempnum[2];
	PV_syear = G_tempnum[1];

	strcpy(t, &G_tempa[1][0]);
	strcat(t, "01010000");
	P_time(t);

	P_format();

	refresh();
	flushinp();

	mep.i = mep.o = mep.a = 0;

	G_act = 0;
	beep();
	G_cltem();
	P_status(NULL);

#undef ifautoset
}

int
G_time(void)
{

	int i;

	i = P_time(&G_tempa[0][0]);

	if (i < 0) {
		G_mess(10);
	} else {
		P_status("Time updated");
	}

}

void
G_repcon(WINDOW * w)
{
	int tx, ty, max = 18;

	werase(w);
	wattrset(w, A_NORMAL);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 0, 0, "Latest Time:");
	wattrset(w, A_NORMAL);
	wmove(w, 1, 1);
	G_showt(w, PV_shijian);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 2, 0, "Slots State:");
	wattrset(w, (!P_space() || P_space() == PV_capacity) ? A_BOLD : A_NORMAL);
	mvwprintw(w, 3, 1, "%ld/%ld", PV_capacity - P_space(), PV_capacity);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 4, 0, "Car Park Name:");
	wattrset(w, A_NORMAL);
	getyx(w, ty, tx);
	max = G_max(tx, max);
	mvwprintw(w, 5, 1, "%s", PV_file);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 6, 0, "Year Span:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 7, 1, "%d-%d", PV_syear, PV_lastyear);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 8, 0, "Car Park Size:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 9, 1, "%ld*%ld*%ld", PV_z, PV_y, PV_x);
	getyx(w, ty, tx);
	max = G_max(tx, max);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 10, 0, "Free Parking Time:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 11, 1, "%d min", PV_free);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 12, 0, "Price:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 13, 1, "%.3lf per min", PV_norprice);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 14, 0, "Special Price:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 15, 1, "%.3lf per min", PV_disprice);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 16, 0, "Special Period:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 17, 1, "%02d:%02d-%02d:%02d", PV_disstart[0], PV_disstart[1], PV_disend[0], PV_disend[1]);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 18, 0, "Cumulative Amount:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 19, 1, "%.3lf", PV_threshold);
	wattrset(w, A_UNDERLINE);
	mvwaddstr(w, 20, 0, "Award Money:");
	wattrset(w, A_NORMAL);
	mvwprintw(w, 21, 1, "%.3lf", PV_gift);
}

void
G_gcon(int a, int b)
{
	static int x = 0, y = 0;
	int i, j;
	int max = 18;

	if (mep.i == 6 && mep.o == 7)
		return;

	G_repcon(G_con);

	x += b;
	y += a;


	if (abs(a) > 1 || abs(b) > 1)
		x = y = 0;
	if (y > 21)
		y = 21;
	if (y < 0)
		y = 0;
	if (x >= max)
		x = max - 1;
	if (x < 0)
		x = 0;

	int uy, ux;

	if (G_right - 1 <= G_con_x - x) {
		ux = COLS - 1;
	} else {
		ux = G_con_x - x + G_vdiv;
		G_regclean(1, ux + 1, G_cont_y - 1);
	}
	if (G_cont_y - 1 <= G_con_y - y) {
		uy = G_cont_y - 1;

	} else {
		uy = G_con_y - y;
		G_regclean(uy + 1, G_vdiv + 1, G_cont_y - 1);
	}

	pnoutrefresh(G_con, y, x, 1, G_vdiv + 1, uy, ux);

}

int
P_progress(char *c, double m)
{

	char b[7];
	int i;
	sprintf(b, " %.1lf%%", m * 100);
	if (strlen(c) + strlen(b) > COLS)
		return -1;
	m *= COLS;

	move(LINES - 1, 0);
	clrtoeol();
	attrset(A_DIM);
	hline(' ', COLS);
	attrset(A_REVERSE);
	for (i = 0; i < m; i++)
		addch(' ');
	attrset(A_NORMAL);
	move(LINES - 1, G_middle(0, COLS - 1, 6 + strlen(c)));
	for (i = 0; i < strlen(c); ++i) {
		if ((inch() & A_ATTRIBUTES) == A_REVERSE)
			attrset(A_REVERSE);
		else
			attrset(A_DIM);
		addch(c[i]);
	}
	for (i = 0; i < strlen(b); ++i) {
		if ((inch() & A_ATTRIBUTES) == A_REVERSE)
			attrset(A_REVERSE);
		else
			attrset(A_DIM);
		addch(b[i]);
	}
	attrset(A_NORMAL);
	move(LINES - 3, 0);
	refresh();

}

int
G_showm(WINDOW * w, char *Z_temp_s)
{

	long Z_i, Z_p, Z_o, Z_c;
	char Z_s[41];

	strcpy(Z_s, Z_temp_s);
	Z_p = strlen(Z_s);
	do
		--Z_p;
	while ((Z_s[Z_p] != '.') && (Z_p > 0));
	if ((Z_p == 0) && (Z_s[Z_p] == '.'))
		wprintw(w, "%s", Z_s);
	if (Z_p == 0)
		Z_p = strlen(Z_s);

	Z_c = 0;
	for (Z_i = Z_p - 1; Z_i > 0; --Z_i) {
		++Z_c;
		if ((Z_c % 3) == 0) {
			Z_p = strlen(Z_s);
			for (Z_o = Z_p; Z_o >= Z_i; --Z_o)
				Z_s[Z_o + 1] = Z_s[Z_o];
			Z_s[Z_i] = ',';
		}
	}
	wprintw(w, "%s", Z_s);
}

int
G_showt(WINDOW * w, char *dd)
{

	switch (strlen(dd)) {

		case 4:
		wprintw(w, "%c%c:%c%c", dd[0], dd[1], dd[2], dd[3]);
		break;

	case 8:
		wprintw(w, "%c%c%c%c-%c%c-%c%c", dd[0], dd[1], dd[2], dd[3], dd[4], dd[5], dd[6], dd[7]);
		break;

	case 12:
		wprintw(w, "%c%c%c%c-%c%c-%c%cT%c%c:%c%cZ", dd[0], dd[1], dd[2], dd[3], dd[4], dd[5], dd[6], dd[7], dd[8], dd[9], dd[10], dd[11]);
		break;

	default:
		break;
	}

	return 0;

}

int
G_melast(void)
{
	int i;
	if ((i = mlasts[IND_LAST(mep.i, mep.a)]) >= 0)
		return i;

	for (i = 1; G_ip(mep.i, mep.a, i) != 2; ++i);
	return mlasts[IND_LAST(mep.i, mep.a)] = i;
}

int
G_key(int a)
{

	char *b = "Switch Window";
	if (mep.i == 7)
		b = "NULL";

#define putk(l,r) G_keyput(u, d, (l), (r), b)

	move(LINES - 2, 0);
	if (a == 0) {
		clrtoeol();
		return 0;
	}
	switch (G_act) {

	case 0:

		if (res.i) {
			if (mep.i == 1 && mep.a == 2)
				G_keyput("Scroll Up", "Scroll Down", "Cancel", "Proceed", b);

			else if (res.f == &G_save)
				G_keyput("Scroll Up", "Scroll Down", "Finish", "Save", b);
			else
				G_keyput("Scroll Up", "Scroll Down", "Finish", "Finish", b);
		} else {
			char *u, *d;
			switch (G_pos) {
			case 0:
				u = "Cursor Last";
				d = "Cursor Down";
				break;
			case 1:
				u = "Cursor Up";
				d = "Cursor Down";
				break;
			case 2:
				u = "Cursor Up";
				d = "Cursor First";
				break;
			}

			switch (G_typ) {

			case G_tme:

				if (G_it(mep.i, mep.a, 0) != 0) {
					putk("NULL", "Submenu");
				} else if ((mep.i == 7 || mep.i == 0) && mep.a == 0) {
					putk("NULL", "Submenu");
				} else {
					putk("Parent Menu", "Submenu");
				}
				break;

			case G_tcl:
				putk("NULL", "Clear");
				break;
			case G_tfu:
				putk("NULL", "Act");
				break;
			default:
				putk("NULL", "Input");
			}

		}
		break;

	case 1:

		G_keyput("Scroll Up", "Scroll Down", "Scroll Left", "Scroll Right", b);
		break;

	case 2:

		G_keyput("Scroll Up", "Scroll Down", "Scroll Left", "Scroll Right", b);
		break;

	}
#undef putk
}

static void
putkey(const char *k, const char *s, int l)
{
#define AL 15
	l -= 2 + strlen(k);
	if (l < 0) {
		l = 0;
	}
	char a[AL + 1];
	strcpy(a, s);
	a[(l > AL) ? AL : l] = 0;
	attrset(A_NORMAL);
	addch('[');
	attrset(A_UNDERLINE);
	addstr(k);
	attrset(A_NORMAL);
	addch(']');
	addstr(a);
#undef AL
}

int
G_keyput(const char *q, const char *w, const char *e, const char *r, const char *t)
{

	int i = 6;
	int u = COLS / i;
	int l = 0;
	char b[6];

	move(LINES - 2, 0);
	clrtoeol();

	putkey("UP", q, COLS - l);
	l += u;
	move(LINES - 2, l);

	putkey("DOWN", w, COLS - l);
	l += u;
	move(LINES - 2, l);

	putkey("LEFT", e, COLS - l);
	l += u;
	move(LINES - 2, l);

	putkey("RIGHT", r, COLS - l);
	l += u;
	move(LINES - 2, l);

	putkey("TAB", t, COLS - l);

	if (mep.i != 7 && res.i != 1 && G_act == 0) {
		l += u;
		move(LINES - 2, l);
		sprintf(b, "1-%d", G_melast() + 1);
		putkey(b, "Go To Item N", COLS - l);
	}
}

int
G_ghis(int yy, int xx)
{

	static short int y = 0, x = 0;

	y += yy;
	x += xx;

	if (abs(yy) > 1 || abs(xx) > 1) {
		x = 0;
		y = (sim.y > LINES - 4 - G_cont_y) ? sim.y - LINES + 5 + G_cont_y : 0;
	}
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x > sim.x - 1)
		x = sim.x - 1;
	if (y > sim.y - 2)
		y = sim.y - 2;

	pnoutrefresh(G_his, y, x, G_cont_y + 1, G_vdiv + 1, LINES - 4, COLS - 1);

}

int
G_whis(char *l, char *d, char *p, int s)
{

	if (G_his_y - sim.y + 1 < 4) {
		wmove(G_his, 0, 0);
		winsdelln(G_his, -4);
		sim.y -= 4;
	}
	if (s == 0) {
		mvwprintw(G_his, sim.y, 0, "%s(I)", l);
		sim.x = G_max(sim.x, strlen(l) + 3);
	}
	if (s == 1) {
		mvwprintw(G_his, sim.y, 0, "%s(O)", l);
		sim.x = G_max(sim.x, strlen(l) + 4);
	}
	mvwaddch(G_his, sim.y + 1, 0, '@');
	sim.x = G_max(sim.x, 18);
	G_showt(G_his, d);
	if (strchr(p, '.') == NULL)
		mvwprintw(G_his, sim.y + 2, 0, "#");
	else
		mvwprintw(G_his, sim.y + 2, 0, "$");
	wprintw(G_his, "%s", p);
	sim.x = G_max(sim.x, strlen(p));
	sim.y += 4;
	G_ghis(2, 2);

}

int
G_format(void)
{
	P_format();
	flushinp();
	beep();
	P_status(NULL);
}

int
G_para(void)
{

	res.i = 1;
	res.f = NULL;
	G_resu(2, NULL);
	G_repcon(G_res);

	wmove(G_res, 0, 0);
	winsdelln(G_res, 1);
	whline(G_res, ACS_HLINE, COLS);
	mvwaddstr(G_res, 0, G_middle(0, G_vdiv - 1, strlen("PARAMETERS")), "PARAMETERS");
	mvwaddch(G_res, 0, G_vdiv, ACS_RTEE);
	res.l = G_con_y;
}

int
G_readm(void)
{

	FILE *re;

	char *e;
	int i;

	if (res.i == 2) {
		mep.i = mep.o;
		mep.o = 0;
	} else {
		if ((re = fopen("README", "r")) == NULL) {
			P_status("File README not found");
			fclose(re);
			return -1;
		}
		e = (char *) malloc(COLS + 1);
		for (i = 0; fgets(e, COLS, re) != NULL; ++i);
		if (i + 12 >= G_res_y) {
			P_status("README too long");
			fclose(re);
			free(e);
			return -1;
		}
		re = fopen("README", "r");
		res.i = 1;
		res.f = &G_readm;
		mep.o = mep.i;
		mep.i = 6;
		G_resu(2, "README");

		wmove(G_res, 1, 0);
		whline(G_res, '=', G_res_x);
		wmove(G_res, 2, 0);
		waddstr(G_res, "Program Files Version");
		G_rever(G_res, 4);
		wmove(G_res, 12, 0);
		whline(G_res, '=', G_res_x);

		for (i = 0; fgets(e, COLS, re) != NULL; ++i) {
			e[strlen(e) - 1] = '\0';
			mvwprintw(G_res, i + 13, 0, "%s", e);
		}
		fclose(re);
		free(e);
		res.l = i + 12;
	}
}

int
G_save(void)
{

	FILE *f;
	char a[91], op[185] = "cp ";
	int i;

	if (res.i == 0) {
		P_status("Result too long. please input file path to save it.");
		refresh();
	} else {
		P_status("Please input file path");
		refresh();
	}
	while (1) {

		i = G_typin(&G_checkfp, "File path to save:", 90, "", a);

		if (i < 0) {
			P_status(NULL);
			break;
		}
		f = fopen(a, "r");
		if (f != NULL) {
			P_status("File may already exists. OK to proceed?");
			refresh();
			res.i = 1;
			G_keyput("Cancel", "NULL", "Re-enter", "Proceed", "NULL");
			res.i = 0;

			while ((i = getch()) != KEY_LEFT && i != KEY_RIGHT && i != KEY_UP);

			P_status(NULL);
			refresh();
			if (i == KEY_LEFT)
				continue;
			if (i == KEY_UP)
				break;
		}
		f = fopen(a, "w");
		if (f == NULL) {
			P_status("File path not accessible");
			refresh();
			continue;
		}
		fclose(f);
		strcat(op, PV_tempfi);
		strcat(op, " ");
		strcat(op, a);
		system(op);
		P_status("File successfully saved");
		break;

	}

}

int
G_checkfp(int c)
{

	return (isprint(c) && !isspace(c) && c != '-');

}

int
P_ui_c(int c[])
{

	c[0] = versiona;
	c[1] = versionb;
	c[2] = versionc;

}

int
G_rever(WINDOW * w, int l)
{

	int v[7][3], i;
	char *p[] = {
		"earn.c",
		"lxs.c",
		"power.c",
		"record.c",
		"store.c",
		"ui.c",
		"yhz.c",
		"core.h"
	};
	P_earn_c(v[0]);
	P_lxs_c(v[1]);
	P_power_c(v[2]);
	P_record_c(v[3]);
	P_store_c(v[4]);
	P_ui_c(v[5]);
	P_yhz_c(v[6]);

	mvwaddstr(w, l, 0, p[7]);
	mvwprintw(w, l, 10, "%2d.%2d.%2d", c_versiona, c_versionb, c_versionc);
	for (i = 0; i < 7; i++) {
		mvwaddstr(w, l + 1 + i, 0, p[i]);
		mvwprintw(w, l + 1 + i, 10, "%2d.%2d.%2d", v[i][0], v[i][1], v[i][2]);
	}
}

void
G_cleaninit(void)
{
	int i;
	for (i = 0; i < G_MAX_DIM; i++)
		G_eraser.reg[i] = -1;
}

void
G_clean(void)
{
	int i;
	if (G_eraser.nuke) {
		G_eraser.nuke = 0;
		G_cleaninit();
		erase();
	} else {
		for (i = 0; i < LINES; i++) {
			if (G_eraser.reg[i] >= 0) {
				move(i, G_eraser.reg[i]);
				clrtoeol();
				G_eraser.reg[i] = -1;
			}
		}
	}
}

void
G_regclean(int sy, unsigned int sx, unsigned int ey)
{
	if (sy < 0) {
		G_eraser.nuke = 1;
		return;
	}
	if (ey < sy)
		return;

	int i, j;

	for (i = sy; i <= ey; i++) {
		j = G_eraser.reg[i];
		if (j > sx) {
			G_eraser.reg[i] = sx;
		}
	}
}
