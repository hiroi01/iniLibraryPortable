
all:
	make clean
	make -f Makefile_p
	cp -f libinip.a ../lib
	make clean
	make -f Makefile_p LITE=1
	cp -f libiniplite.a ../lib
	make clean

clean:
	make -f Makefile_p clean
	make -f Makefile_p LITE=1 clean

