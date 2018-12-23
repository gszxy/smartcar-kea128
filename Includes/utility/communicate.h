/*
 * communicate.h
 *
 *  Created on: 2018年12月15日
 *      Author: 张笑语
 *
 *      单片机与其它设备（主要是PC机）进行通讯的方法
 */

#ifndef INCLUDES_COMMUNICATE_H_
#define INCLUDES_COMMUNICATE_H_

#include <cstdint>
#include <cstring>
#include "SKEAZ1284.h"
#include "uart.h"


#define VALID_UART_CHANNEL_WITH_BUFFER_1



class UARTCommunicator
{//TODO:实现printf
private://循环缓冲区

	uint8_t rxbufferlen;
	volatile uint8_t *rxbuffer;
	volatile uint8_t *rxb_head_ptr;//head是缓冲区内第一个字符的地址
	volatile uint8_t *rxb_tail_ptr;//tail是循环缓冲区内最后一个字符的地址+1
	volatile uint8_t txbufferlen;
	volatile uint8_t *txbuffer;
	volatile uint8_t *txb_head_ptr;
	volatile uint8_t *txb_tail_ptr;
	inline void DisableNVICIntr()
	{//在中断控制器出屏蔽与当前通道有关的中断
		switch(this->uart->GetChannel())
		{
		case UART_settings::UARTR0:
			NVIC_DisableIRQ(UART0_IRQn);
		break;
		case UART_settings::UARTR1:
			NVIC_DisableIRQ(UART1_IRQn);
		break;
		case UART_settings::UARTR2:
			NVIC_DisableIRQ(UART2_IRQn);
		break;
		}
	}
	inline void EnableNVICIntr()
	{//在中断控制器出屏蔽与当前通道有关的中断
		switch(this->uart->GetChannel())
		{
		case UART_settings::UARTR0:
			NVIC_EnableIRQ(UART0_IRQn);
		break;
		case UART_settings::UARTR1:
			NVIC_EnableIRQ(UART1_IRQn);
		break;
		case UART_settings::UARTR2:
			NVIC_EnableIRQ(UART2_IRQn);
		break;
		}
	}
protected:
	UARTModule *uart;
public:
	UARTCommunicator(uint8_t rx_bufferlen,uint8_t tx_bufferlen,UART_settings::UARTn uartn,bool is_port_remap,uint16_t baud_rate);
	~UARTCommunicator();
	void SendString(char *buffer,uint8_t length/*长度包括终结字符\0*/);
	void SendChar(char send);
	//uint8_t* GetString();
	//TODO: 实现GetString()
	uint8_t GetChar();
	uint8_t PeekChar();
    void CleanRxBuffer();


	/*..............由中断处理函数调用，在发送缓冲区空时发送下一个字符.............*/
    void OnIntrSendNext();
	/*..............由中断处理函数调用，在接收缓冲区满时读取下一个字符.............*/
    void OnIntrRecieveNext();
    /*中断处理函数见intr.h*/


};

/*..这个数组包括已经获没有初始化的三个通道对应的this指针。提供给中断处理函数使用..*/
extern UARTCommunicator *cmtr_uart[];



#endif /* INCLUDES_COMMUNICATE_H_ */
