/*
 * pid.cpp
 *
 *  Created on: 2018年12月23日
 *      Author: 张笑语
 */

#include <cstdint>
#include <cmath>
#include "pid.h"
#include "sensor.h"

/*
 * PIDController Member functions
 */

AngleController*  wAngleController::ac = nullptr;

PIDController::PIDController(uint16_t p,uint16_t i,uint16_t d)
{
	this->p = p;
	this->i = i;
	this->d = d;
	for(int i=0;i<10;i++)
	{
		this->storage[i] = 0;
	}
	this->index = 0;
}

uint16_t PIDController::GetControlOutput(int32_t error)
{
	++index;
	if(index == 10)
		index = 0;
	storage[index] = error;
	int32_t diff;
	if(index == 0)
		diff = storage[0] - storage[9];
	else
		diff = storage[index] - storage[index-1];
	//求微分项结果
    //下面是比例项
	int32_t proportion = error * p;
	//然后是积分项
	int32_t integration = 0;
	for(int i = 0;i<10;i++)
		integration += storage[i];
	integration *= i;//乘以积分项系数

	int32_t final = (diff + proportion + integration)/10000;
	return (uint16_t)(5000+final);
	//TODO:增加保护性截断功能，防止舵机超出允许范围
}


/*
 * SteerController Member functions
 */

int32_t AngleController::DoControl(uint16_t data[])
{
	//data[0 1 2]分别是左、中、右电感的值
	//下面进行归一化
	//将其归一化为一个0-10000的值
	uint16_t standard_value[3];
	for(int i=0;i<3;i++)
	{
		if(data[i]<mins[i])
			mins[i] = data[i];
		if(data[i]>maxes[i])
			maxes[i] = data[i];
		if(maxes[i] ==mins[i] )//避免出现除零
			mins[i]--;
		standard_value[i] = (data[i]-mins[i])*10000/(maxes[i]-mins[i]);
		//下面是提线算法
	}

	//此处暂时只使用左右两个电感,判断车辆状态是否偏出赛道、是否进入上下坡等内容的程序均由状态机完成

	int32_t error = standard_value[0] - standard_value[2];

	int16_t duty_cyc = GetControlOutput(error);
	if(duty_cyc>5450)
		duty_cyc = 5450;
	else if(duty_cyc < 4550)
		duty_cyc = 4550;//舵机限幅
	//TODO:将限幅功能移动到包装的舵机类中
	return duty_cyc;



}
