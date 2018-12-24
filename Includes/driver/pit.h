/*
 * pit.h
 *
 *  Created on: 2018年12月21日
 *      Author: 张笑语
 */

#ifndef INCLUDES_DRIVER_PIT_H_
#define INCLUDES_DRIVER_PIT_H_

#include <cstdint>
#include "system_SKEAZ1284.h"
#include "SKEAZ1284.h"

namespace PITSettings
{
	enum Channel
	{
		channel_0,
		channel_1
	};
}

class PeriodicInterruptTimer
{
private:
	PITSettings::Channel channel;
public:
	PeriodicInterruptTimer(PITSettings::Channel channel);
	inline void SetPeriod(uint32_t period_in_us)
	{
		PIT->CHANNEL[channel].LDVAL  = period_in_us * (SystemCoreClock/1000000);
	}
	inline void EnableIntr()
	{
	    PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK;
	}
	inline void DisableIntr()
	{
	    PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TIE_MASK;
	}
	~PeriodicInterruptTimer()
	{
		this->DisableIntr();
	}
};


class wPIT    //对pit类进行单例模式包装
{//暂时只对外提供对CH0的支持，待弄清各通道相互作用之后再添加另一个通道
private:
	static PeriodicInterruptTimer *pit_ch0 ;
	static inline void PITch0Init()
	{
		pit_ch0 = new PeriodicInterruptTimer(PITSettings::channel_0);
	}
public:
	static inline PeriodicInterruptTimer* GetPitch0()
	{
		if(pit_ch0 == nullptr)
			PITch0Init();
		return pit_ch0;
	}
};






#endif /* INCLUDES_DRIVER_PIT_H_ */
