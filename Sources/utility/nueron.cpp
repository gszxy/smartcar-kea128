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
	int16_t  result;
	result = real_input;

	return result;//返回增量pid输出
}


int16_t SingleNeuronController::GetControlOutput(int16_t error)
{
	int32_t partial;
	int32_t integrate;
	int32_t diff;
	//更新数据：
	for(int i=5;i>=1;i--)
		storage[i] = storage[i-1];
	storage[5] = error;
	if(error>0)
		partial = error*pw_right;//比例项
	else
		partial = error*pw_left;//比例项
	//删除积分项
	diff = (storage[0]+storage[1]+storage[2]-storage[3]-storage[4]-storage[5]) * dw / 3;
	//逐差法求微分项
	int16_t output = ActivationFcn(partial + integrate + diff);
	//过激活函数。除以一万的工作已由激活函数完成，更新输出偏移量
	//保护性截断
	if(output > 520)
		output = 520;
	else if(output < -520)
		output = -520;

	//下面开始Hebb监督学习
	if(error > 0)
	{
		pw_left -= pls * ((error * partial / 10000 )* static_cast<int32_t>(output) ) /10000;
		pw_right += pls * ((error * partial / 10000 )* static_cast<int32_t>(output)) /10000;
		if(pw_left < 800)
			pw_left = 800;
	}
	else
	{
		pw_left += pls * (error * partial * static_cast<int32_t>(output) / 10000) /10000;
		pw_right -= pls * (error * partial * static_cast<int32_t>(output) / 10000) /10000;
		if(pw_right <800)
			pw_right = 800;
	}

	//两次除以10000分别对应比例项和学习速度的万分之一
	//最后输出舵机占空比
	return 5000 + output;
}
