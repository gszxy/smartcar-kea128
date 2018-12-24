/*
 * motor.h
 *
 *  Created on: 2018年12月24日
 *      Author: 张笑语
 */

#ifndef INCLUDES_UTILITY_MOTOR_H_
#define INCLUDES_UTILITY_MOTOR_H_


#include <cstdint>
#include "ftm.h"
#include "gpio.h"

class Motor
{
	GPIOModule *gpio_c;  //控制输出是否被选通的gpio模块
	FlexTimerModule *ftm;//pwm模块
public:
	Motor()
	{

	}
	inline void SetMotorSpeed(uint16_t ftm_duty_cycle)
	{
		ftm->SetDutyCycle(ftm_duty_cycle);
	}
	~Motor()
	{

	}
};

class wMotor  //对电机类进行单例模式包装
{
private:
	static Motor *left_motor;
	static Motor *right_motor ;
	static inline void LeftMotorInit()
	{

	}
	static inline void RightMotorInit()
	{

	}
public:
	static inline Motor* GetLeftMotorObj()
	{
		if(left_motor == nullptr)
			LeftMotorInit();
		return left_motor;
	}
	static inline Motor* GetRightMotorObj()
	{
		if(right_motor == nullptr)
			LeftMotorInit();
		return right_motor;
	}


};
#endif /* INCLUDES_UTILITY_MOTOR_H_ */
