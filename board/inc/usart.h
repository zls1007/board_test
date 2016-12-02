#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "stdio.h"

void USART1_Config(void);
void USART2_Config(void);
void USART3_Config(void);
void UART4_Config(void);
void UART7_Config(void);

int fputc(int ch, FILE *f);
void usart_deinit(void);
void usart_print_str(USART_TypeDef *usart, char *buf);
void usart_print(USART_TypeDef *usart, u8 buf[], u8 size);

#endif

