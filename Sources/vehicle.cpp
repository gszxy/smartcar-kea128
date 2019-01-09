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
#include "neuron.h"
#include "steer.h"
State* RunningState::HandleCommand(uint8_t command)
{
	switch(command)
	{
	case cmd::stop_force:
	case cmd::stop_ready:       //两种状态下均返回停止状态
		return new IdleState();
		break;
//	case 0x03://读取子命令
//		sub_command = g_uartc->GetCharWithExpiration();
//		break;
	default:
		return nullptr;
	}

}

void RunningState::StateChangeOp()
{

	char msg[] = "running...\n";
	g_uartc->SendString(msg,strlen(msg));
	Motor* left = MotorSingletons::GetLeftMotorObj();
	Motor* right = MotorSingletons::GetRightMotorObj();
	left->Run();
	right->Run();
	PeriodicInterruptTimer *pitm = wPIT::GetPitch0();
	pitm->SetPeriod(5000);
	SteerSingleton::GetSteerObj();//调用一下此函数，确保舵机pwm输出被正确初始化并位于中位
}

void RunningState::StateRemainOp()
{
	auto g_sensor = SensorSingletons::GetInductorSensor();
	if(!g_sensor->flag_data_updated)
		return;
	g_sensor->flag_data_updated = false;
	//状态保持时，如果数据已经更新，则执行控制算法
	AngleController *angle_control = wAngleController::GetAngleController();//从单例模式处获取指针
	uint16_t values[3];
	for(uint8_t i=0;i<4;i++)
		values[i] = g_sensor->values[i];
	int32_t steer_yaw = angle_control->DoControl(values);//调用提线算法
	SteerSingleton::GetSteerObj()->SetYaw(steer_yaw);

	//下面将电感数据发送至上位机
	for(uint8_t i=0;i<4;i++)
	{
		g_uartc->SendChar(0xFF - i);//同步字，用于上位机辨识消息
		g_uartc->SendChar(0xFF - i);//同步字，用于上位机辨识消息
		g_uartc->SendChar(static_cast<uint8_t>(values[i]>>8));//高八位
		g_uartc->SendChar(static_cast<uint8_t>(values[i]));//低八位
	}


	//下面开始速度闭环控制
	auto wheels = SensorSingletons::GetWheelSpeedSensor();
	if(wheels->left_speed_is_updated)
	{
		uint16_t spd = wheels->left_wheel_spd;
		MotorSingletons::GetLeftMotorObj()->DoPIDSpdControl(spd);
		wheels->left_speed_is_updated = false;
		g_uartc->SendChar(0xEF);//同步字，用于上位机辨识消息
		g_uartc->SendChar(static_cast<uint8_t>(spd>>8));//高八位
		g_uartc->SendChar(static_cast<uint8_t>(spd));//低八位
	}
	if(wheels->right_speed_is_updated)
	{
		uint16_t spd = wheels->right_wheel_spd;
		MotorSingletons::GetRightMotorObj()->DoPIDSpdControl(spd);
		wheels->right_speed_is_updated = false;
		g_uartc->SendChar(0xEE);//同步字，用于上位机辨识消息
		g_uartc->SendChar(static_cast<uint8_t>(spd>>8));//高八位
		g_uartc->SendChar(static_cast<uint8_t>(spd));//低八位
	}

}

void RunningState::StateExitOp()
{//退出运动状态时，要使舵机归中，电机停止
	Motor* left = MotorSingletons::GetLeftMotorObj();
	Motor* right = MotorSingletons::GetRightMotorObj();
	left->Halt();
	right->Halt();
	SteerSingleton::GetSteerObj()->SetYaw(0);
}
