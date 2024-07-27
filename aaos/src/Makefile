include ../config.mk

INCLUDE = -I../include $(LIBUMPS_INCLUDE)
LDFLAGS = -T elf32ltsmip.h.umpscore.x
LDLIBS = $(LIBUMPS_PATH)/crtso.o $(LIBUMPS_PATH)/libumps.o $(LIBRARY)

LIB_SRC = string.c termio.c msg.c tcb.c io_event.c p2test.c
LIB_OBJ = $(patsubst %.c,%.o,$(LIB_SRC))

TARGET_OBJ = $(patsubst %,%.o,$(TARGETS))

all: $(LIBRARY) $(TARGETS)

clean:
	-rm -f *.o *.umps $(LIBRARY) $(TARGETS)

$(LIBRARY): $(LIB_OBJ)
	$(AR) -rc $(LIBRARY) $(LIB_OBJ)

$(LIB_OBJ): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $<

$(TARGETS): %: %.o $(LIBRARY)
	$(LD) $(LDFLAGS) $< $(LDLIBS) -o $@
	$(ELF2UMPS) -k $@

$(TARGET_OBJ): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $<

.PHONY: all clean
