/*
 * Copyright (C) Michael Goldweber, Enrico Cataldi, Mauro Morsiani 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
                                                                                

/*********************************P1TEST.C*******************************
 *
 *      Test program for TCB Queue Manager and Message Queue Manager
 *
 *      Produces progress messages on terminal 0 in addition
 *              to the array ``okbuf[]''
 *              Error messages will also appear on terminal 0 in
 *              addition to the array ``errbuf[]''.
 *
 *              Aborts as soon as an error is detected.
 *
 */
                                                                                
#include "tcb.h"
#include "msg.h"
#include "termio.h"
#include "arch.h"

#define EOS			'\0'
#define FALSE 0
#define TRUE 1
#define HIDDEN static

HIDDEN char okbuf[2048];
HIDDEN char errbuf[256];
HIDDEN char *msgTail;

HIDDEN unsigned int display(char *src);
HIDDEN unsigned int panicMessage(char *src);

tcb_t   *threadp[MAXTHREADS], *thread1, *thread2, *threadQ, *firstThread, *lastThread, *middleThread;
msg_t	*msgp[MAXMESSAGES], *msg1, *msg2, *msgq, *firstMessage, *lastMessage, *middleMessage;

int main(void){

	int index;
	msgTail = okbuf;

	initTcbs();
	display("initialized thread control blocks   \n");

	/* Here we check for ThreadBLK allocation */
	for (index = 0; index < MAXTHREADS; index++) {
		if ((threadp[index] = allocTcb()) == NULL)
			panicMessage("allocTcb: unexpected NULL   \n");
	}

	if ((thread1=allocTcb()) != NULL) {
		panicMessage("allocTcb: allocated more than MAXTHREADS entries   \n");
	}

	display("allocTcb ok   \n");

	display("freeing 10 entries   \n");

	/* return the last 10 entries back to free list */
	for (index = 10; index < MAXTHREADS; index++)
		freeTcb(threadp[index]);

	display("freed 10 entries   \n");

	/* create a 10-element thread queue */
	threadQ = mkEmptyThreadQ();

	if (!emptyThreadQ(threadQ))
		panicMessage("emptyThreadQ: unexpected FALSE   \n");

	display("inserting...   \n");

	for (index = 0; index < 10; index++) {
		if ((thread2 = allocTcb()) == NULL)
			panicMessage("allocTcb: unexpected NULL while insert   \n");

		switch (index) {
			case 0:
				firstThread = thread2;
				break;
			case 5:
				middleThread = thread2;
				break;
			case 9:
				lastThread = thread2;
				break;
			default:
				break;
		}
		insertThread(&threadQ, thread2);
	}
	display("inserted 10 elements   \n");
	if (emptyThreadQ(threadQ))
		panicMessage("emptyThreadQ: unexpected TRUE   \n");

	/* Check outProc and headProc */
	if (headThread(threadQ) != firstThread)
		panicMessage("headThread failed   \n");

	thread2 = outThread(&threadQ, firstThread);

	if (thread2 == NULL || thread2 != firstThread)
		panicMessage("outThread failed on first entry   \n");

	freeTcb(thread2);

	thread2 = outThread(&threadQ, middleThread);

	if (thread2 == NULL || thread2 != middleThread)
		panicMessage("outThread failed on middle entry   \n");

	freeTcb(thread2);

	if (outThread(&threadQ, threadp[0]) != NULL)
		panicMessage("outThread failed on nonexistent entry   \n");

	display("outThread ok   \n");

	/* Check if removeProc and insertProc remove in the correct order */
	display("removing...   \n");

	for (index = 0; index < 8; index++) {
		if ((thread2 = removeThread(&threadQ)) == NULL)
			panicMessage("removeThread: unexpected NULL   \n");
		freeTcb(thread2);
	}

	if (thread2 != lastThread)
		panicMessage("removeThread: failed on last entry   \n");

	if (removeThread(&threadQ) != NULL)
		panicMessage("removeThread: removes too many entries   \n");

	if (!emptyThreadQ(threadQ))
		panicMessage("emptyThreadQ: unexpected FALSE   \n");

	display("insertThread, removeThread and emptyThreadQ ok   \n");
	display("thread queues module ok      \n");
	display("checking thread trees...\n");

	if (!emptyChild(threadp[2]))
		panicMessage("emptyChild: unexpected FALSE   \n");

	/* make threadp[1] through threadp[9] children of threadp[0] */
	display("inserting...   \n");

	for (index = 1; index < 10; index++) {
		insertChild(threadp[0], threadp[index]);
	}
	
	display("inserted 9 children   \n");

	if (emptyChild(threadp[0]))
		panicMessage("emptyChild: unexpected TRUE   \n");

	/* Check outChild */
	thread2 = outChild(threadp[1]);

	if (thread2 == NULL)
		panicMessage("outChild failed on first child A  \n");
	else if (thread2 != threadp[1])
		panicMessage("outChild failed on first child B  \n");

	thread2 = outChild(threadp[4]);

	if (thread2 == NULL || thread2 != threadp[4])
		panicMessage("outChild failed on middle child   \n");

	if (outChild(threadp[0]) != NULL)
		panicMessage("outChild failed on nonexistent child   \n");

	display("outChild ok   \n");

	/* Check removeChild */
	display("removing...   \n");

	for (index = 0; index < 7; index++) {
		if ((thread2 = removeChild(threadp[0])) == NULL)
			panicMessage("removeChild: unexpected NULL   \n");
	}

	if (removeChild(threadp[0]) != NULL)
		panicMessage("removeChild: removes too many children   \n");

	if (!emptyChild(threadp[0]))
		panicMessage("emptyChild: unexpected FALSE   \n");

	display("insertChild, removeChild and emptyChild ok   \n");
	display("thread tree module ok      \n");
	for (index = 0; index < 10; index++) 
		freeTcb(threadp[index]);

	/* Check for the message management */
	initMsgs(); 
	display("initialized messages   \n");	
	/* Check for allocation */

	for (index = 0; index < MAXMESSAGES; index++){
		if ((msgp[index] = allocMsg()) == NULL)
			panicMessage("allocMsg: unexpected NULL   \n");
	}

	if (allocMsg() !=NULL )
		panicMessage("allocMsg : allocated more than MSGMAX entries   \n");
	

	display("allocMsg ok   \n");

	/* return the last 10 entries back to free list */
	for (index = 10; index < MAXMESSAGES; index++)
		freeMsg(msgp[index]);

	display("freed 10 entries   \n");

	/* create a 10-element msg queue */
	msgq = mkEmptyMessageQ();
	display("initialized msg queue   \n");
	display("inserting...   \n");
	for (index = 0; index < 10; index++){

		if ((msg1 = allocMsg()) == NULL)
			panicMessage("allocMsg: unexpected NULL while insert   \n");

		switch (index){
			case 0:
				firstMessage = msg1;
				break;
			case 5:
				middleMessage = msg1;
				break;
			case 9:
				lastMessage = msg1;
				break;
			default:
				break;
		}
		insertMessage(&msgq,msg1);
	}
	display("inserted 10 msgs   \n");

	/* Check headMessage */
	if (headMessage(msgq) != firstMessage)
		panicMessage("headMessage failed   \n");

	/* Check if removeMessage and insertMessage remove in the correct order */
	display("removing...   \n");
	for (index = 0; index < 10; index++){
		if ((msg1 = popMessage(&msgq,NULL)) == NULL)
			panicMessage("popMessage : unexpected NULL   \n");
		freeMsg(msg1);
	}
	
	if (msg1 != lastMessage)
		panicMessage("popMessage : failed on last entry   \n");

	if (popMessage(&msgq,NULL) != NULL)
		panicMessage("popMessage : removes too many entries   \n");

	for (index = 0; index < 5; index++) {
		if ((msg1 = allocMsg()) == NULL)
			panicMessage("allocMsg : unexpected NULL while insert   \n");

		switch (index){
			case 0:
				firstMessage = msg1;
				break;
			case 3:
				middleMessage = msg1;
				break;
			case 4:
				lastMessage = msg1;
				break;
			default:
				break;
		}
		threadp[index] = allocTcb();
		msg1->m_sender = threadp[index];
		msg1->message=index;
		insertMessage(&(threadp[index]->inbox),msg1);
	}

	display("inserted 5 messages   \n");

	msg1 = popMessage(&(threadp[0]->inbox),NULL);
	
	if (msg1 == NULL || msg1 != firstMessage) 
		panicMessage("insertMessage failed in first entry\n");

	freeMsg(msg1); freeTcb(threadp[0]);

	msg1 = popMessage(&(threadp[3]->inbox),NULL);

	if (msg1 == NULL || msg1 != middleMessage)
		panicMessage("insertMessage failed on middle entry   \n");

	freeMsg(msg1); freeTcb(threadp[3]);

	if (!emptyMessageQ(threadp[0]->inbox))
		panicMessage("emptyMessageQ failed on nonexistent entry   \n");


	msg1 = popMessage(&(threadp[1]->inbox),NULL);
	freeMsg(msg1); freeTcb(threadp[1]);
	msg1 = popMessage(&(threadp[2]->inbox),NULL);
	freeMsg(msg1); freeTcb(threadp[2]);
	msg1 = popMessage(&(threadp[4]->inbox),NULL);
	freeMsg(msg1); freeTcb(threadp[4]);

	display("insertMessage, popMessage emptyMessageQ ok   \n");

	/*check pushMessage */
	msgq = mkEmptyMessageQ();
	msg1 = allocMsg();
	lastMessage = msg1;
	insertMessage(&msgq,msg1);
	msg1 = allocMsg();
	firstMessage = msg1;
	pushMessage(&msgq,msg1);
	if (headMessage(msgq) != firstMessage)
                panicMessage("headMessage wrong because pushMessage failed   \n");

	display("pushMessage ok   \n");

	display("So Long and Thanks for All the Fish\n");

	return(0);

}

HIDDEN unsigned int display(char *src){
	char *ptr;

	/* Here we save over the okbuf all messages displayed, for debugging */
	for (ptr = src;*ptr != EOS;*msgTail++=*ptr++);

	*msgTail=' ';

	/* Here we print the message on the terminal 0 */
	return term_puts(0, src);
}

HIDDEN unsigned int panicMessage(char *src){
	char *ptr,*src2;
	ptr = errbuf; src2=src;
	/* Here we save over the error_buffer the message, for debugging */
	while (*src2 != EOS)
		*ptr++=*src2++;

	/* Here we print the message on the terminal 0 */
	return term_puts(0, src);
	PANIC();
	/* will never return */
	return FALSE;
}
