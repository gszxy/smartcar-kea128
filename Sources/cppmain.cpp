#include <cstdint>
#include "communicate.h"
#include "gpio.h"
#include "state_machine.h"
#include "cppmain.h"
#include "glb_var.h"
#include "UART.h"
#include "core_cm0plus.h"
using namespace std;


int _cppmain()
{

	bool vehicle_has_piority_command = false;
	//下面是模块初始化工作
	//串口测试模块初始化（临时）
	g_test_adc = new ADCModule(ADC_CHANNEL_AD0,ADC_8BIT);
	//串口初始化
	g_uartc = new UARTCommunicator(16, 32, UART_settings::UARTR0, false, 19200);
	//状态机初始化
	StateMachine *fsm = new StateMachine();
	char msg[] = "startup\n";
	g_uartc->SendString((uint8_t*)(&(msg[0])),9);
	while(1)//程序主循环
	{
		/*............检查通讯区是否有相应命令到达............*/
		uint8_t command = g_uartc->GetChar();
		/*...........检查中断处理程序是否已经更新数据..........*/


		/*.............状态机更新并执行相应操作.............*/
		if(command!=0)
			fsm->SendCommand(command);

	}

	while(1);
	return 0;//永不返回
}



volatile void __attribute__((interrupt ("IRQ"))) UART0_IRQHandler()
{
	NVIC_DisableIRQ(UART0_IRQn);
	if(cmtr_uart[0] ==nullptr)
	{
		uint32_t some = UARTx[0]->S1 ;
		NVIC_EnableIRQ(UART0_IRQn);
		return;
	}

	if(UARTx[0]->S1 & UART_S1_TDRE_MASK);//如果发送寄存器为空
		cmtr_uart[0]->OnIntrSendNext();
	if(UARTx[0]->S1 & UART_S1_RDRF_MASK)//如果接受寄存器为满
		cmtr_uart[0]->OnIntrRecieveNext();
	NVIC_EnableIRQ(UART0_IRQn);
}

volatile void __attribute__((interrupt ("IRQ"))) UART1_IRQHandler()
{
	NVIC_DisableIRQ(UART1_IRQn);
	if(cmtr_uart[1] ==nullptr)
	{
		uint32_t some = UARTx[1]->S1 ;
		NVIC_EnableIRQ(UART0_IRQn);
		return;
	}


	if(UARTx[1]->S1 & UART_S1_TDRE_MASK);//如果发送寄存器为空
		cmtr_uart[1]->OnIntrSendNext();
	if(UARTx[1]->S1 & UART_S1_RDRF_MASK)//如果接受寄存器为满
		cmtr_uart[1]->OnIntrRecieveNext();
	NVIC_EnableIRQ(UART1_IRQn);
}

volatile void __attribute__((interrupt ("IRQ"))) UART2_IRQHandler()
{
	NVIC_DisableIRQ(UART2_IRQn);
	if(cmtr_uart[2] ==nullptr)
	{
		uint32_t some = UARTx[2]->S1 ;
		NVIC_EnableIRQ(UART0_IRQn);
		return;
	}


	if(UARTx[2]->S1 & UART_S1_TDRE_MASK);//如果发送寄存器为空
		cmtr_uart[2]->OnIntrSendNext();
	if(UARTx[1]->S1 & UART_S1_RDRF_MASK)//如果接受寄存器为满
		cmtr_uart[2]->OnIntrRecieveNext();
	NVIC_EnableIRQ(UART1_IRQn);
}


