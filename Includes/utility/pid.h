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
#include "neuron.h"

class PIDController
{
private:
/*PID Parameters*/
	uint16_t p_left;
	uint16_t p_right;
	uint16_t pls = 0;
	uint16_t i;
	uint16_t d;
	uint16_t former_output;
	double storage[5];//记录的历史数据量
	uint8_t index;
public:
	PIDController(uint16_t p,uint16_t i,uint16_t d);
	/*
	 * PID控制主算法函数
	 * 输入量：误差。须在外部使用各算法编写完成，获得误差后调用函数。
	 * 输出量:一个0-10000的数，用于控制PWM波的占空比
	 */
	uint16_t GetControlOutput(double error);
	void SetParameters(uint16_t p,uint16_t i ,uint16_t d )//参数单位：万分之一
	{
		p_left = p_right = p;
		this->i = i;
		this->d = d;
	}
};

class AngleController : public PIDController
{
private:
/*historical data begins*/
	uint16_t maxes[3];
	uint16_t mins[3];
/*historical data ends*/
public:
	/*
	 * 舵机提线算法函数
	 * 提供归一化功能
	 */

	int32_t DoControl(uint16_t data[]);
	AngleController() : PIDController(3000,0,40)
	{
		for(int i = 0;i<3;i++)
		{
			maxes[i]=0x0;
			mins[i]=0x0;//最小值已禁用
		}
	}
};

class wAngleController    //角度控制类的单例模式包装
{
private:
	static AngleController *ac;
	static inline void acInit()
	{
		ac = new AngleController();
	}
public:
	static inline AngleController* GetAngleController()
	{
		if(ac == nullptr)
			acInit();
		return ac;
	}
};


#endif /* INCLUDES_UTILITY_PID_H_ */
