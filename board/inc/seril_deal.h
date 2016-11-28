#ifndef __SERIL_DEAL_H
#define __SERIL_DEAL_H
#include "stm32f4xx.h"
#include "usart.h"

void serilDeal(u8  *buf);
void buf4_deal(void);
void SerilSendBack(char *msg_type, u8 result);
void SerilSendState(char *msg_type);


void Seril4_Send(u8 d1, u8 d2);

#endif
