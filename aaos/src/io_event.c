#include "adt.h"
#include "arch.h"
#include "io_event.h"

DEF_ADT_SMM(ioevent_t, IOEvent, MAX_IO_EVENT)
DEF_ADT_QUEUE(ioevent_t, IOEvent, next, prev)
