/*
 * state_machine.c
 *
 *  Created on: 2018年12月15日
 *      Author: 张笑语
 *
 *      注意：本文件中并不实现StateMachine中涉及到小车具体控制的函数
 */

#include <cstdint>
#include <cstring>
#include "state_machine.h"
#include "glb_var.h"
#include <cstdio>
using namespace std;

State* IdleState::HandleCommand(uint8_t command)
{
	switch(command)
	{
	case cmd::test_uart:
		return new TestUARTState();
		break;
	case cmd::test_adc:
		return new TestADConverterState();
	default:
		return nullptr;
	}
}

State* TestUARTState::HandleCommand(uint8_t command)
{
	if(command == cmd::stop_force)//仅当命令为强制停止（0）时，程序可以退出串口测试状态。否则将收到的数据原样发还给上位机
		return new IdleState();
	g_uartc->SendChar(command);
	//注:g前缀是统一定义在glb_var.h内的全局变量
	return nullptr;
}
State* TestUARTState::HandleInput()
{

}
void TestUARTState::StateChangeOp()
{
	char msg[] = "entered UART test mode\n";
	g_uartc->SendString(msg,strlen(msg) + 1);
}
State* IdleState::HandleInput()
{

}


StateMachine::StateMachine()
{
	this->current_state = new IdleState();
}
StateMachine::~StateMachine()
{
	delete current_state;
}
void StateMachine::SendCommand(uint8_t command)
{
	State* nxtstate = this->current_state->HandleCommand(command);
	if(nxtstate != nullptr)
	{
		delete this->current_state;
		this->current_state = nxtstate;
		this->current_state->StateChangeOp();
	}
	this->current_state->StateRemainOp();
}



/*
 * member function of ADC Conveter test state
 */

State* TestADConverterState::HandleCommand(uint8_t command)
{
	if(command == cmd::stop_force)//仅当命令为强制停止（0）时，程序可以退出ADC测试状态。否则将收到的数据原样发还给上位机
		return new IdleState();
}

void TestADConverterState::StateChangeOp()
{
	char msg[] = "entered ADC CHANNEL 0 test mode\n";
	g_uartc->SendString(msg,strlen(msg) + 1);
}

void TestADConverterState::StateRemainOp()
{
	g_test_adc->StartConversion(8);
	uint32_t counter = 0;
	uint16_t result;
	do
	{
		result = g_test_adc->TryFetchResult();
		++counter;
	}while(result > 0xFFF);
	size_t strl;
	char msg[50];
	strl = snprintf(msg,50,"Convert Finished\n result:%d",result);
	g_uartc->SendString(msg,strl);
	strl = snprintf(msg,50,"\ntime consumption:%d",counter);
	g_uartc->SendString(msg,strl);

	//注:g前缀是统一定义在glb_var.h内的全局变量
}


void TestADConverterState::StateExitOp()
{
	char msg[] = "left ADC CHANNEL 0 test mode\n";
	g_uartc->SendString(msg,strlen(msg) + 1);
}









