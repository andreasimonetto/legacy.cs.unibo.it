VERSION = 1.0
CPPFLAGS = -Wall -O2 -I. -Iinclude/
LDFLAGS = -Lmge/ -Lmre/ -lGL -lGLU -lX11 -lm
CC = g++

.PHONY: sys/libsys.a mge/libmge.a mre/libmre.a doc clean

uplezz: uplezz.cc images.cc main.cc sys/libsys.a mge/libmge.a mre/libmre.a

sys/libsys.a:
	make -C sys

mge/libmge.a:
	make -C mge

mre/libmre.a:
	make -C mre

doc: clean
	cd doc && doxygen

clean:
	rm -f *.o uplezz
	make -C mre clean
	make -C mge clean
	make -C sys clean
