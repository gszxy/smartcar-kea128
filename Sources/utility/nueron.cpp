/*
 * nueron.cpp
 *
 *  Created on: 2018年12月30日
 *      Author: 张笑语
 */

#include <cstdint>
#include <cmath>
#include "neuron.h"

SingleNeuronController* sSNC::sncptr = nullptr;

int16_t SingleNeuronController::ActivationFcn(int32_t input_sum)   //激活函数
{
	//为避免浮点数运算，pid系数均为1万倍。此处要除以一万得电机控制的增量
	//在子类中重写此函数以实现对不同设备（电机和舵机的控制）
	//此处为舵机控制
	int16_t real_input = input_sum/10000;
	uint32_t abs_input = abs(real_input);
	int16_t  result;
	if(abs_input < 10)//死区
		result = 0;
	else if(abs_input < 150 )//线性区，其临界值有待修改
		result = real_input;
	else
	{
		if(real_input > 0)
			result = real_input * 5 - 600;  //bangbang区
		else//real_input<0
			result = real_input * 5 + 600;
	}

	return result;//返回增量pid输出
}


int16_t GetControlOutput(int16_t error)
{
	int32_t partial;
	int32_t integrate;
	int32_t diff;
	//更新数据：
	for(int i=5;i>=1;i++)
		storage[i] = storage[i-1];
	storage[5] = error;
	partial = error*pw;//比例项
	for(int i=0; i<6; i++)
	{
		integrate = iw * storage[i];
	}
	diff = (storage[0]+storage[1]+storage[2]-storage[3]-storage[4]-storage[5]) * dw / 3;
	//逐差法求微分项
	int16_t former_output = ActivationFcn(partial + integrate + diff) + former_output;
	//过激活函数。除以一万的工作已由激活函数完成，更新输出偏移量
	//保护性截断
	if(former_output > 450)
		former_output = 450;
	else if(former_output < -450)
		former_output = -450;

	//下面开始神经元的Hebb监督学习
	pw += pls * (error * partial * abs(former_output) / 10000) /10000;
	//两次除以10000分别对应比例项和学习速度的万分之一
	iw += ils * (error * integrate * abs(former_output) / 10000) /10000;
	dw += dls * (error * diff * abs(former_output) / 10000) /10000;

	//最后输出舵机占空比
	return 5000 + former_output;
}
