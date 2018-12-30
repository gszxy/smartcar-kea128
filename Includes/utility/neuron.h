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
	int16_t pls;
	int16_t ils;
	int16_t dls;    //比例，积分，微分项的<--学习速度-->
	int32_t pw;
	int32_t iw;
	int32_t dw;     //权重系数（weight）
	int16_t former_output;//上一次的输出偏移量。作增量pid控制。
	int16_t ActivationFcn(int32_t input_sum);   //激活函数
public:
	int16_t GetControlOutput(int16_t error);
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
