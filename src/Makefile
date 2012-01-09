#ATMega328p
all:
	make -C arduinocore -f Makefile all
	make -f Makefile.mk all
	
clean:
	make -C arduinocore -f Makefile clean
	make -f Makefile.mk clean
	
program:
	make -f Makefile.mk program
	
#ATMega168p
all-168:
	make -C arduinocore -f Makefile.168 all
	make -f Makefile.mk.168 all
	
clean-168:
	make -C arduinocore -f Makefile.168 clean
	make -f Makefile.mk.168 clean
	
program-168:
	make -f Makefile.mk.168 program


		