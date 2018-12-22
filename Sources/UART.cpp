#include <cstdint>
#include "SKEAZ1284.h"
#include "kea128_config.h"
#include "UART.h"

using namespace std;
volatile UART_Type *UARTx[3] = {UART0,UART1,UART2};
uint8_t UARTModule::chn_occupation_flag = 0x0;//静态变量初始化

UARTModule::UARTModule(UART_settings::UARTn uartn, bool is_port_remap, uint16_t baud_rate,bool enable_recieve_intr ,bool enable_txreg_empty_intr)
{
	//TODO:检查通道是否已经被占用，并且在被占用报错
	this->is_pin_remap = is_port_remap;
	UARTModule::chn_occupation_flag += (1 << static_cast<uint8_t>(uartn));
	this->channel = uartn;

	//下面代码抄袭于龙邱库。之后处理一下
	uint8_t temp;
	uint16_t  sbr;
	sbr = (uint16_t)(BUS_CLK_HZ/(16*baud_rate));
	//UART 波特率 = UART 模块时钟 / (16 × (SBR[12:0] + BRFA))
	//不考虑 BRFA 的情况下， SBR = UART 模块时钟 / (16 * UART 波特率)
	if(sbr > 0x1FFF) sbr = 0x1FFF;                                       //SBR 是 13bit，最大为 0x1FFF
	switch(uartn)
	{
	case UART_settings::UARTR0:
		//通过对SIM寄存器的设置设置引脚复用，对应管脚由GPIO变化为串口通讯接口
	    SIM->SCGC |=  SIM_SCGC_UART0_MASK;
	    if (!is_port_remap)
	    	SIM->PINSEL &= ~SIM_PINSEL_UART0PS_MASK;   //RX--B0 ,TX--B1
	    else
	    	SIM->PINSEL |= SIM_PINSEL_UART0PS_MASK;   //PTA3 TX ,PTA2 RX
	    //TODO:以下三行的含义是什么？为什么UART1和UART2没有？研究一下。
	    SIM->SOPT0 &= ~SIM_SOPT0_TXDME_MASK ;
	    SIM->SOPT0 &= ~SIM_SOPT0_RXDFE_MASK ;
	    SIM->SOPT0 &= ~SIM_SOPT0_RXDCE_MASK ;
	break;
	case UART_settings::UARTR1:
	    SIM->SCGC |=  SIM_SCGC_UART1_MASK;
	    if (!is_port_remap)
	    	SIM->PINSEL1 &= ~SIM_PINSEL1_UART1PS_MASK;   //X--C6 ；TX--C7
	    else
	    	SIM->PINSEL1 |= SIM_PINSEL1_UART1PS_MASK ;  //PTF3 TX ,PTF2 RX
	break ;
	case UART_settings::UARTR2:
	    SIM->SCGC |=  SIM_SCGC_UART2_MASK;
	    if (!is_port_remap)
	      SIM->PINSEL1 &= ~SIM_PINSEL1_UART2PS_MASK;   //RX--D6 ；TX--D7
	    else
	      SIM->PINSEL1 |= SIM_PINSEL1_UART2PS_MASK ;  //PTI1 TX ,PTI0 RX
	break;
	  }
	  temp = UARTx[uartn]->BDH&~UART_BDH_SBR(0X1F);
	  UARTx[uartn]->BDH = temp|UART_BDH_SBR(((sbr & 0x1F00) >> 8)) ;
	  UARTx[uartn]->BDL = (uint8_t)(sbr&UART_BDL_SBR_MASK);
	  UARTx[uartn]->C1  = 0 ;
	  UARTx[uartn]->C2  = (0
	                       |UART_C2_TE_MASK
	                       |UART_C2_RE_MASK
	 );
	  if(enable_recieve_intr)
		  this->EnableIntrOnRecieve();
	  else
		  this->DisableIntrOnRecieve();
	  if(enable_txreg_empty_intr)
		  this->EnableIntrOnTxRegEmpty();
	  else
		  this->DisableIntrOnTxRegEmpty();
}

void UARTModule::SendChar(uint8_t send)
{
	 UARTx[this->channel]->D=send;
}
UARTModule::~UARTModule()
{
	UARTModule::chn_occupation_flag ^= (1 << static_cast<uint8_t>(this->channel));
}
uint8_t UARTModule::RecieveChar()
{
	  uint8_t recieve;
	  while(( UARTx[this->channel]->S1 & UART_S1_RDRF_MASK)==0);	/* Wait for receive buffer to be full*/								/* Read UART2_S1 register*/
	  	  recieve= UARTx[this->channel]->D;								/* Read received data*/
	  return recieve;
}


