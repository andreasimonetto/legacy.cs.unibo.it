CPPFLAGS = -Wall -O3 -I. -I..
CC = g++
AR = ar

all: libmge.a

test: libmge.a

libmge.a: mge.o 
	$(AR) rc libmge.a *.o

clean:
	rm -f test *.o libmge.a
