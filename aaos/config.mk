# Project settings
DISTNAME = aaos
VERSION = 1.0beta4
LIBRARY = libaa.a
SRCDIR = src
DOCDIR = doc
TESTDIR = test
KERNEL_CORE = kernel.core.umps
KERNEL_STAB = kernel.stab.umps
TARGETS = kernel p1test

# uMPS settings
LIBUMPS_PATH = /usr/lib/uMPS
LIBUMPS_INCLUDE = -I/usr/include/uMPS
ELF2UMPS = umps-elf2umps
MKDEV = umps-mkdev

# Compiler, linker, archiver settings
CFLAGS = -ansi -pedantic -Wall -c -nodefaultlibs -nostdlib
LDFLAGS = -nodefaultlibs -nostdlib -T elf32ltsmip.h.umpscore.x 
CC = mipsel-linux-gcc 
LD = mipsel-linux-ld
AR = mipsel-linux-ar
