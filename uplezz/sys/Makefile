CPPFLAGS = -Wall -O3 -I. -I..
LDFLAGS =
CC = g++
AR = ar

all: libsys.a

libsys.a: sys.o 
	$(AR) rc libsys.a *.o

clean:
	rm -f *.o libsys.a
