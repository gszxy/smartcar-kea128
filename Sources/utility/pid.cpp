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
#include "neuron.h"

/*
 * PIDController Member functions
 */



AngleController*  wAngleController::ac = nullptr;

PIDController::PIDController(uint16_t p,uint16_t i,uint16_t d)
{
	this->p_left = this->p_right =  p;
	this->i = i;
	this->d = d;
	for(int i=0;i<10;i++)
	{
		this->storage[i] = 0;
	}
	this->index = 0;
}

uint16_t PIDController::GetControlOutput(double error)
{
	++index;
	if(index == 5)
		index = 0;
	storage[index] = error;
	int32_t diff;
	if(index == 0)
		diff = storage[0] - storage[4];
	else
		diff = storage[index] - storage[index-1];
	//求微分项结果
    //下面是比例项
	int32_t proportion;
	if(error>0)
		proportion = error*p_right;//比例项
	else
		proportion = error*p_left;//比例项
	//然后是积分项
	int32_t integration = 0;
	for(int i = 0;i<10;i++)
		integration += storage[i];
	integration *= i;//乘以积分项系数
	diff *= this->d;
	int32_t final = (diff + proportion + integration)/1000;


		p_left -= pls * ((error )* static_cast<int32_t>(final - former_output) ) /10000;
		p_right += pls * ((error )* static_cast<int32_t>(final- former_output)) /10000;
		if(p_left < 800)
			p_left = 800;
		if(p_left > 2400)
			p_left = 2400;


	former_output = final;
	return (uint16_t)(final);
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
		standard_value[i] = (data[i]-mins[i])*1000/(maxes[i]-mins[i]);
		//下面是提线算法
	}

	//此处暂时只使用左右两个电感,判断车辆状态是否偏出赛道、是否进入上下坡等内容的程序均由状态机完成

	double error_front = pow(300 * sqrt( abs(standard_value[0] - standard_value[2]) ) / (standard_value[0] + standard_value[2]) , 2) ;
	double error_hind = pow(300 * sqrt( abs(standard_value[1] - standard_value[3]) ) / (standard_value[1] + standard_value[3]) , 2);

	if(standard_value[0] < standard_value[2])
		error_front*=-1;
	if(standard_value[1] < standard_value[3])
		error_hind*=-1;

	double error = 1.1 * error_front - error_hind;
	//目标位置-当前位置，多出来的0.1为试验性的前馈环节
	int16_t yaw = GetControlOutput(error);

	return yaw;



}
