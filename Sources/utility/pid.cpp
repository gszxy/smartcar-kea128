/*
 * pid.cpp
 *
 *  Created on: 2018年12月23日
 *      Author: 张笑语
 */

#include <cstdint>
#include "pid.h"


/*
 * PIDController Member functions
 */

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
