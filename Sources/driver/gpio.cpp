/*
 * gpio.c
 *
 *  Created on: 2018年12月15日
 *      Author: 张笑语
 */

#include <cstdint>
#include "gpio.h"

using namespace std;
volatile GPIO_Type *GPIOx[3]={GPIOA,GPIOB,GPIOC};

GPIOModule::GPIOModule(PTxn port,bool direction_is_output)
{
	if (direction_is_output)   //输出
	{
	    GPIO_PDDR_REG(GPIOX_BASE(port)) |= (1 << PTn(static_cast<uint8_t>(port)));  //设置端口方向为输出
	    this->SetState(0);   //调用gpio_set函数，设定引脚输出状态
	}
	else
	{
	    GPIO_PDDR_REG(GPIOX_BASE(port)) &= ~(1 << PTn(port));  //设置端口方向为输入
	    //GPIO_PIDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));  //设置端口方向为输入
	    //龙邱的库里为什么要连续设置两次呢？先注释掉试试有没有问题。
	}
	this->port = port;
}

GPIOModule::~GPIOModule()
{
	//现在的析构函数似乎没什么好做的事
}

