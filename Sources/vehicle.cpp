/*
 * vehicle.cpp
 *
 *  Created on: 2018年12月24日
 *      Author: 张笑语
 */

/*
 * 本文件是状态机内各运行状态的实现文件
 */

#include <cstdint>
#include "state_machine.h"
#include "sensor.h"   //从此文件中的全局变量获得传感器数据
#include "pid.h"      //从此文件中获得pid控制方法
#include "pit.h"      //通过此文件中的单例模式获得pit定时中断功能
#include "glb_var.h"
#include "communicate.h" //串口通讯需要
#include "motor.h"
State* RunningState::HandleCommand(uint8_t command)
{
	switch(command)
	{
	case cmd::stop_force:
	case cmd::stop_ready:       //两种状态下均返回停止状态
		return new IdleState();
		break;
	//case cmd::test_ftm://读取子命令,暂未实现
	//	sub_command = g_uartc->GetCharWithExpiration();
	//	break;
	default:
		return nullptr;
	}
}

void RunningState::StateChangeOp()
{
	char msg[] = "running...\n";
	g_uartc->SendString(msg,strlen(msg));
	Motor* left = wMotor::GetLeftMotorObj();
	Motor* right = wMotor::GetRightMotorObj();
	left->SetMotorSpeed(1900);
	right->SetMotorSpeed(1900);
	PeriodicInterruptTimer *pitm = wPIT::GetPitch0();
	pitm->SetPeriod(5000);
	g_steer_pwm->SetPWMParam(329,5000);
	g_steer_pwm->EnablePWMOutput();
}

void RunningState::StateRemainOp()
{
	if(!g_sensor->flag_data_updated)
		return;
	g_sensor->flag_data_updated = false;
	//状态保持时，如果数据已经更新，则执行控制算法
	AngleController *angle_control = wAngleController::GetAngleController();//从单例模式处获取指针
	uint16_t values[3];
	for(uint8_t i=0;i<3;i++)
		values[i] = g_sensor->values[i];
	int32_t duty_cyc = angle_control->DoControl(values);//调用提线算法
	g_steer_pwm -> SetDutyCycle(duty_cyc);//设置舵机占空比

	//下面将电感数据发送至上位机
	for(uint8_t i=0;i<3;i++)
	{
		g_uartc->SendChar(0xFF - i);//同步字，用于上位机辨识消息
		g_uartc->SendChar(0xFF - i);//同步字，用于上位机辨识消息
		g_uartc->SendChar(static_cast<uint8_t>(values[i]>>8));//高八位
		g_uartc->SendChar(static_cast<uint8_t>(values[i]));//低八位
	}
}

void RunningState::StateExitOp()
{//退出运动状态时，要使舵机归中，电机停止
	Motor* left = wMotor::GetLeftMotorObj();
	Motor* right = wMotor::GetRightMotorObj();
	left->SetMotorSpeed(0);
	right->SetMotorSpeed(0);
	g_steer_pwm -> SetDutyCycle(5000);
}
