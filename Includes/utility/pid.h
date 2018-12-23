/*
 * pid.h
 *
 *  Created on: 2018年12月22日
 *      Author: 张笑语
 */

#ifndef INCLUDES_UTILITY_PID_H_
#define INCLUDES_UTILITY_PID_H_

#include <cstdint>
#include "sensor.h"



class PIDController
{
private:
/*PID Parameters*/
	uint32_t p;
	uint32_t i;
	uint32_t d;
	uint32_t storage[10];
	uint8_t index;
public:
	PIDController(uint32_t p,uint32_t i,uint32_t d);
	/*
	 * PID控制主算法函数
	 * 输入量：误差。须在外部使用提线算法编写完成，获得误差后调用函数。
	 * 输出量:一个0-10000的数，用于控制PWM波的占空比
	 */
	uint16_t GetControlOutput(uint32_t error);
	void SetParameters(uint32_t p,uint32_t i,uint32_t d);
};

class AngleController : private PIDController
{
private:
/*historical data begins*/
	uint16_t left_max;
	uint16_t left_min;
	uint16_t mid_max;
	uint16_t mid_min;
	uint16_t right_max;
	uint16_t right_min;
/*historical data ends*/
public:
	void DoControl(InductorData *data);
};

extern AngleController *angle_controller;

#endif /* INCLUDES_UTILITY_PID_H_ */
