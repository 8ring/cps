OBJECTS = main.o ui.o store.o record.o power.o lxs.o yhz.o earn.o

cps: $(OBJECTS)
	cc -o cps $(OBJECTS) -lncurses

$(OBJECTS): core.h

clean: 
	rm -f $(OBJECTS)

nuke: clean
	rm -f cps

readme:
	fmt -w 55 README > README_
	mv -f README_ README
	wc -l README

