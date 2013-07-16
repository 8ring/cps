#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "ui.h"

struct Z_car *Z_cc;

char *PV_mess[] = {
	"",
	"Invalid car park name",
	"Folder or file already exists",
	"Invalid car park size. [1 - " Z_max_capacity_cs "]",
	"Invalid time period. [1 - 4000)",
	"Special time period error",
	"Invalid cumulative discount policy",
	"Licence cannot be empty",
	"Time cannot be empty",
	"Customer cannot be empty",
	"Time earlier than latest time",
	"Time exceeds preset span",
	"Vehicle already exist",
	"Car park is full",
	"Vehicle not found",
	"No vehicle found",
	"Lower bound larger than higher one",
	"No record found",
	"Time later than latest time",
	"CPS profile damaged",
};

int
main(int argc, char **argv)
{
	G_menu();
	unsigned int flag = ~0;
	if(argc == 2 && strcmp(*(argv+1), "-f") == 0)
		flag ^= 1;

	Z_cc = (struct Z_car *) calloc(Z_max_capacity + 1, sizeof(struct Z_car));
	if(!Z_cc) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	if(!(G_uinit() & flag)) {
		G_larg();
		return(0);
	}
	G_quit();
}
