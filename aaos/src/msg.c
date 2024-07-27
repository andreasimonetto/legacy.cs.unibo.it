#include "msg.h"
#include "tcb.h"
#include "adt.h"

/* */
DEF_ADT_SMM(msg_t, Msg, MAXMESSAGES)

/* */
DEF_ADT_QUEUE(msg_t, Message, m_next, m_prev) 

/* */
void pushMessage(msg_t **mq, msg_t *m_ptr)
{
	if(mq && *mq)
	{
	msg_t *q = headMessage(*mq);

		insertMessage(q ? &q : mq, m_ptr);
	}
}

/* */
msg_t* popMessage(msg_t **mq, tcb_t *t_ptr)
{
const int threadId = getTcbID(t_ptr);
const int queueId = mq ? getMsgID(*mq) : -1;
msg_t *m_ptr;
	
	if(queueId < 0) {
		/* MSGQ pointer not valid */
		return NULL;
	}
	
	if(!t_ptr) {
		return outMessage(mq, headMessage(*mq));
	}
	else if(threadId < 0) {
		/* TCB pointer not valid */
		return NULL;
	}
	
	m_ptr = headMessage(*mq);
	do {
		if(m_ptr -> m_sender == t_ptr)
			return outMessage(mq, m_ptr);

		m_ptr = m_ptr -> m_next;
	} while(m_ptr != headMessage(*mq));
	
	/* Sender not found. */
	return NULL;
}
