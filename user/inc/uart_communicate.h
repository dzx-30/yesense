#pragma once

#include "main.h"
#include "stdbool.h"
#include "analysis.h"

//UART_HandleTypeDef* huart;

//void USART1_IRQHandler(void);

//#define HEAD_CHAR1 0x59
//#define HEAD_CHAR2 0x53

typedef struct uart_communicate
{
	uint8_t HEAD_FLAG;
	uint8_t RX_FLAG;
	short Data_len;
	uint8_t Data_Yesense[98];
	int res;
	int BodyX, BodyY, BodyZ;
	int flagAveage;
	int j;
	int angleAveage;
	int angleXA;
//	unsigned char head[2];
//	unsigned char rx_data[50];
//	unsigned char write;
//	bool get_head;
//	UART_HandleTypeDef* huart;
}uart_communicate;

void uart_communicate_init(uart_communicate* uc);
void uart_communicate_receive(uart_communicate* uc,uint8_t c);
void G_output_infoSet(void);
