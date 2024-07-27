#ifndef _INIT_H
#define _INIT_H

/* Initial (test) function */
void test();

/* typedefs and macros for compiling p2test */
typedef unsigned cpu_t;

#define STCK(v) (v = get_tod())
#define MsgSend(code, dest, payload) msg_send(dest, payload)
#define MsgRecv(code, source, reply) msg_recv(source, reply)
#define termprint(str, term) term_puts(term, str)

#endif
