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
	FTMSettings::Channels channel;
/*additional channels start*/
	//一个FTM通道的各个模块可以输出同频率不同占空比的PWM信号，因此由一些额外的channel以供单独设置各个channel
	//此功能尚未实现
	FTMSettings::Channels *module_all_channels;
/*private functions used in constructor begin*/
	void PinSet(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type);


/*private functions end*/
public:
	FlexTimerModule(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type);
	void SetPWMParam(uint32_t frequency,uint32_t duty_cycle/*占空比，该数除以10000为占空比(小数)*/);
	inline void EnablePWMOutput()
	{
		FTMx[this->module]->CNT = 0;
		//向计数器内写入任意值，重置为CNTIN中给定的初始值。修改0为其他数不能改变计数器初值！
		FTMx[this->module]->SC = FTM_SC_CLKS(1)|FTM_SC_PS(2) ;
		//FTM_SC_CLKS：向模块提供系统时钟，4分频  10MHz
	}
	inline void DisablePWMOutput()
	{
		FTMx[this->module]->SC = FTM_SC_CLKS(0);
		//不向计数器提供任何时钟，终止计数
	}
	~FlexTimerModule();

};


#endif /* INCLUDES_DRIVER_FTM_H_ */
