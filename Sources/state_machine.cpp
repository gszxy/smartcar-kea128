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
#include "ftm.h"
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
	case cmd::test_ftm:
		return new TestFTMState();
	case cmd::start:
		return new RunningState();
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
		this->current_state->StateExitOp();
		delete this->current_state;
		this->current_state = nxtstate;
		this->current_state->StateChangeOp();
	}
	this->current_state->StateRemainOp();
}



/*
 * member function of ADC Converter test state
 */

State* TestADConverterState::HandleCommand(uint8_t command)
{
	if(command == cmd::stop_force)//仅当命令为强制停止（0）时，程序可以退出ADC测试状态。
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
	strl = snprintf(msg,50,"Conversion Finished\n result:%d",result);
	g_uartc->SendString(msg,strl);

	//注:g前缀是统一定义在glb_var.h内的全局变量
}


void TestADConverterState::StateExitOp()
{
	char msg[] = "left ADC CHANNEL 0 test mode\n";
	g_uartc->SendString(msg,strlen(msg) + 1);
}


/*
 * Member functions of TestFTMState
 */

State* TestFTMState::HandleCommand(uint8_t command)
{
	uint8_t sub_command = 0x0;
	switch(command)
	{
	case cmd::stop_force:
		return new IdleState();
		break;
	case cmd::test_ftm://读取子命令
		sub_command = g_uartc->GetCharWithExpiration();
		break;
	default:
		return nullptr;
	}
	//分析子命令
	uint8_t param[2];
	uint16_t param_to_set;
	switch(sub_command)
	{
	case 1:         //频率设置
		param[0] = g_uartc->GetCharWithExpiration();
		param[1] = g_uartc->GetCharWithExpiration();
		param_to_set = (param[0]<<8)+param[1];//构造16位频率设置(大端对齐)
		g_steer_pwm->SetFrequency(param_to_set);
		break;
	case 2:         //占空比设置
		param[0] = g_uartc->GetCharWithExpiration();
		param[1] = g_uartc->GetCharWithExpiration();
		param_to_set = (param[0]<<8)+param[1];//构造16位占空比设置(大端对齐)
		g_steer_pwm->SetDutyCycle(param_to_set);
		break;
	case 3:         //停止输出
		g_steer_pwm->DisablePWMOutput();
		break;
	case 4:
		g_steer_pwm->EnablePWMOutput();
		break;
	default:
		return nullptr;
	}
	char msg[]="successfully loaded settings\n";
	g_uartc->SendString(msg,strlen(msg));
	return nullptr;
}

void TestFTMState::StateChangeOp()
{
	char msg[] = "Entered FTM Test Mode\n";
	g_uartc->SendString(msg,strlen(msg));
}

