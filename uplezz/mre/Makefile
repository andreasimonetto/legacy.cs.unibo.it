CPPFLAGS = -Wall -O3 -I. -I.. -I/usr/X11R6/include
LDFLAGS = -lGL -lGLU -lX11 -lm
CC = g++
AR = ar

all: libmre.a

MeshScanObj.cc: MeshScanObj.l
	flex -o MeshScanObj.cc MeshScanObj.l

MeshScanMtl.cc: MeshScanMtl.l
	flex -o MeshScanMtl.cc MeshScanMtl.l

libmre.a: mre.o alpha.o MeshScanObj.o MeshScanMtl.o
	$(AR) rc libmre.a *.o

../sys/libsys.a:
	make -C ../sys/

test: libmre.a ../sys/libsys.a test.cc

clean:
	rm -f *.o MeshScanObj.cc MeshScanMtl.cc libmre.a test

