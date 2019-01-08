/*
 * neuron.h
 *
 *  Created on: 2018年12月30日
 *      Author: 张笑语
 */

#ifndef INCLUDES_UTILITY_NEURON_H_
#define INCLUDES_UTILITY_NEURON_H_

#include <cstdint>
#include <cmath>

class SingleNeuronController
{
private:
	int16_t storage[6];      //存储过去的误差值
	int16_t pls = 5;
	int16_t ils = 0;
	int16_t dls = 0;    //比例，积分，微分项的<--学习速度-->
	int32_t pw_left = 1400;//左转，即error<0时的比例项系数
	int32_t pw_right = 1400;
	int32_t iw = 0;
	int32_t dw = 0;     //权重系数（weight）
	int16_t former_output;//上一次的输出偏移量。作增量pid控制。
	int16_t ActivationFcn(int32_t input_sum);   //激活函数
public:
	int16_t GetControlOutput(int16_t error);
	SingleNeuronController()
	{
		for(int i=0;i<6;i++)
			storage[i] = 0;
	}
};




//对神经元类进行单例模式包装
class sSNC
{
	static SingleNeuronController* sncptr;
	static inline void init()
	{
		sncptr = new SingleNeuronController();
	}
public:
	static inline SingleNeuronController* GetSteerSingleNeuronController()
	{
		if(sncptr == nullptr)
			init();
		return sncptr;
	}
};

#endif /* INCLUDES_UTILITY_NEURON_H_ */
