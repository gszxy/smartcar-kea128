#pragma once


#include <cstdint>
#include "adc.h"
#include "pit.h"
#include "ftm.h"





class InductorSensor
{
private:
	//num_of_adc = 3

public://由于中断函数的缘故，不得不把所有函数全部暴露为公有
	   //以后考虑用单例模式的方法实现包装
	ADCModule *adcm;                      //数组，包含各ad转换器
	volatile uint16_t values[3] = {0,0,0};
	volatile bool flag_data_updated = false;
	volatile uint8_t current_adc_m;       //正在进行转换的adc模块
	bool sys_has_ongoing_convert = 0;
	InductorSensor();
	~InductorSensor();
	void StartConvert();
};




class WheelSpeedSensor
{
private:
	FTMSettings::ExternalClockPin pins[2];//左、右编码器信号输入对应引脚
public:
	FlexTimerModule *ftm;
	volatile uint16_t left_wheel_spd;
	volatile uint16_t right_wheel_spd;
	volatile bool left_speed_is_updated = false;
	volatile bool right_speed_is_updated = false;
	volatile bool current_wheel_is_left_one = false;
	WheelSpeedSensor(FlexTimerModule *module,FTMSettings::ExternalClockPin left,FTMSettings::ExternalClockPin pin_right)
	{
		ftm = module;
		pins[0] = left;
		pins[1] = right;
	}

	inline void GetAndClearCount()//中断函数调用下列函数，其余函数不要使用。
	{									//有pit中断处理函数将此处获取的值赋给wheel_speed
		ftm->DisableClock();
		uint16_t value = ftm->GetCounterValue();
		ftm->ClearCounter();
		if(current_wheel_is_left_one)
		{
			left_wheel_spd = value;
			left_speed_is_updated = true;
		}
		else
		{
			right_wheel_spd = value;
			right_speed_is_updated = true;
		}
	}

	inline void LeftStart()
	{
		current_wheel_is_left_one = true;
		ftm->SetExternalClockSource(pins[0]);
		ftm->EnableExternalClock();
	}
	inline void RightStart()
	{
		current_wheel_is_left_one = false;
		ftm->SetExternalClockSource(pins[1]);
		ftm->EnableExternalClock();
	}
};


class SensorSingletons
{
private:
	static InductorSensor *inductors;
	static WheelSpeedSensor *wheels;
	static void InducInit()
	{
		inductors = new InductorSensor();
	}
	static void WheelInit()
	{
		auto ftm = new FlexTimerModule(FTMSettings::mFTM1);
		wheels = new WheelSpeedSensor(ftm,FTMSettings::clk_1_E0,FTMSettings::clk_2_E7);
	}
public:
	static inline InductorSensor* GetInductorSensor()
	{
		if(inductors == nullptr)
			InducInit();
		return inductors;
	}
	static inline WheelSpeedSensor* GetWheelSpeedSensor()
	{
		if(wheels == nullptr)
			InducInit();
		return wheels;
	}
};



#ifdef __cplusplus
extern "C"
{
#endif

volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler();
volatile void __attribute__((interrupt ("IRQ"))) ADC_IRQHandler();

#ifdef __cplusplus
}
#endif






