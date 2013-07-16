OBJECTS = main.o ui.o store.o record.o power.o lxs.o yhz.o earn.o

cps: $(OBJECTS)
	cc -o cps $(OBJECTS) -lncurses

$(OBJECTS): core.h

clean: 
	rm -f $(OBJECTS)

nuke: clean
	rm -f cps

readme:
# fmt has different versions, use caution. Check man page for -l or -w or whatever
## Plan 9:
#	fmt -l 55 README > README_
## (Open)BSD, Linux, &c
#	fmt -w 55 README > README_

#	mv -f README_ README
#	wc -l README
	@echo 1>&2 Modify the makefile for target readme.
