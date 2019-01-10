/*
 * ftm.h
 *
 *  Created on: 2018年12月18日
 *      Author: 张笑语
 *      只实现了FTM的PWM输出功能
 */

#ifndef INCLUDES_DRIVER_FTM_H_
#define INCLUDES_DRIVER_FTM_H_


#include <cstdint>
#include "system_SKEAZ1284.h"
#include "SKEAZ1284.h"


namespace FTMSettings
{
	enum Modules
	{
		mFTM0,
		mFTM1,
		mFTM2
	};

	enum Channels
	{
		FTM_CH0,
		FTM_CH1,
		FTM_CH2,
		FTM_CH3,
		FTM_CH4,
		FTM_CH5,
	};
	enum PortRemapType
	{
		remap_none,
		remap_first,
		remap_second
	};
	enum ExternalClockPin
	{
		clk_0_A5 = 0x0,
		clk_1_E0 = 0x1,
		clk_2_E7 = 0x2
	};
}


/*
通道名                              默认引脚                      重映射引脚[first,second]
FTM0_CH0      PTA0         PTB2
FTM0_CH1      PTA1         PTB3
FTM1_CH0      PTC4         PTH2
FTM1_CH1      PTC5         PTE7
FTM2_CH0      PTC0         PTH0 , PTF0
FTM2_CH1      PTC1         PTH1 , PTF1
FTM2_CH2      PTC2         PTD0 , PTG4
FTM2_CH3      PTC3         PTD1 , PTG5
FTM2_CH4      PTB4         PTG6
FTM2_CH5      PTB5         PTG7

请注意不同模块可用的通道数目不同
*/

extern FTM_Type * FTMx[];

class FlexTimerModule
{
private:
	FTMSettings::Modules module;
	uint16_t frequency = 329;//PWM输出模式下的频率
/*private functions end*/
public:
	FlexTimerModule(FTMSettings::Modules module);
	FlexTimerModule(FTMSettings::Modules module, FTMSettings::ExternalClockPin clock_pin);
	inline void EnableBusClock()//总线时钟，用于pwm输出。
	{
		FTMx[this->module]->SC = FTM_SC_CLKS(1)|FTM_SC_PS(2) ;
		//FTM_SC_CLKS：向模块提供系统时钟，4分频  10MHz
	}
	inline void EnableExternalClock()//外部时钟，用于输入计数。若使用第一个构造函数后调用此函数，将因为无有效时钟触发硬件错误中断
	{
		  FTMx[this->module]->SC = FTM_SC_PS(0)|FTM_SC_CLKS(3);	            //分频系数	0
		  //FTM_SC_CLKS:向模块提供外部时钟，不分频
	}
	inline void DisableClock()//在输出模式下，使用这两个函数控制PWM波的有无
	{
		FTMx[this->module]->SC = FTM_SC_CLKS(0);
		//不向计数器提供任何时钟，终止计数
	}
	inline void ClearCounter()
	{
		FTMx[this->module]->CNT = 0;
		//向计数器内写入任意值，重置为CNTIN中给定的初始值。修改0为其他数不能改变计数器初值！
	}
	inline uint16_t GetFrequency()
	{
		return this->frequency;
	}
	inline void SetExternalClockSource(FTMSettings::ExternalClockPin clock_pin)
	{
		SIM->PINSEL &= ~SIM_PINSEL_FTM0CLKPS_MASK;    //清除外部时钟引脚选择
		if(clock_pin == FTMSettings::clk_0_A5)       //开启内部上拉，避免时钟输入引脚上出现高阻态
			PORT->PUE0 |= (uint32_t)(1<<5);
		else if(clock_pin == FTMSettings::clk_1_E0)
			PORT->PUE1 |= (uint32_t)(1<<0);
		else if(clock_pin == FTMSettings::clk_2_E7)
			PORT->PUE1 |= (uint32_t)(1<<7);
		SIM->PINSEL |= SIM_PINSEL_FTM0CLKPS(clock_pin);       //选择外部时钟输入引脚
	}

	~FlexTimerModule()
	{

	}
	inline FTMSettings::Modules GetModuleNo()
	{
		return this->module;
	}
	//在PWM输出模式下设置输出频率
	void SetFrequency(uint16_t freq);


	//获取计数器内的值。两种模式下均有效，但主要用于输入计数时读取值
	inline uint16_t GetCounterValue()
	{
		return FTMx[this->module]->CNT;
	}
};


class FlexTimerChannel
{
private:
	FTMSettings::Channels channel;
	FlexTimerModule *module;
	/*private functions used in constructor begin*/
	void PinSet(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type);
public:
	FlexTimerChannel(FlexTimerModule *module,FTMSettings::Channels channel_no,FTMSettings::PortRemapType port_remap_type);
	void SetDutyCycle(uint16_t duty_cyc);
	inline void EnablePWMOutput()
	{
		  //下面设置通道的控制寄存器CnSc。它设置了一个边沿对齐，正极性PWM，详见用户手册379页表26-69（以下各位的真值表）
		  FTMx[module->GetModuleNo()]->CONTROLS[channel].CnSC |= (0
		                                       |FTM_CnSC_ELSA_MASK
		                                  //   |FTM_CnSC_ELSB_MASK  //低电平有效脉冲，计数器达到match(CnV)之后输出高电平
		                                  //   |FTM_CnSC_MSA_MASK
		                                       |FTM_CnSC_MSB_MASK   //边缘对齐PWM
		                                  //   |FTM_CnSC_CHIE_MASK  //不开中断
		                                  //   |FTM_CnSC_CHF_MASK   //不重置标志位
		                                       );
	}
	inline void DisablePWMOutput()
	{
		 FTMx[module->GetModuleNo()]->CONTROLS[channel].CnSC = 0;
	}
};


#endif /* INCLUDES_DRIVER_FTM_H_ */
