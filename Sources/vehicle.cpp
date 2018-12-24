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
State* RunningState::HandleCommand()
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
	//default:
	//	return nullptr;
	}
}

void RunningState::StateRemainOp()
{
	InductorData *data = g_sensor->inductor_data;
	if(!data->flag_data_updated)
		return;
	//状态保持时，如果数据已经更新，则执行控制算法
	AngleController *angle_control = wAngleController::GetAngleController();//从单例模式处获取指针
	int32_t duty_cyc = angle_control->DoControl(data);//调用提线算法
	g_steer_pwm -> SetDutyCycle(duty_cyc);//设置舵机占空比
}

void RunningState::StateExitOp()
{//退出运动状态时，要使舵机归中，电机停止
	Motor* left = wMotor::GetLeftMotorObj();
	Motor* right = wMotor::GetRightMotorObj();
	left->SetMotorSpeed(0);
	right->SetMotorSpeed(0);
	g_steer_pwm -> SetDutyCycle(5000);
}
