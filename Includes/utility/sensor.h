#pragma once


#include <cstdint>
#include "adc.h"
#include "pit.h"
#include "ftm.h"
#include "kbi.h"

using namespace KBISettings;


class InductorSensor
{
private:
	//num_of_adc = 3

public://由于中断函数的缘故，不得不把所有函数全部暴露为公有
	   //以后考虑用单例模式的方法实现包装
	ADCModule *adcm;                      //数组，包含各ad转换器
	volatile uint16_t values[4] = {0,0,0,0};
	volatile bool flag_data_updated = false;
	volatile uint8_t current_adc_m;       //正在进行转换的adc模块
	bool sys_has_ongoing_convert = 0;

	InductorSensor();
	~InductorSensor();
	void StartConvert();
};




class WheelSpeedSensor
{
public:
	KBIModule *kbil;
	KBIModule *kbir;
	KBISettings::KBI_PTxn left_pin;
	KBISettings::KBI_PTxn right_pin;

	volatile uint16_t left_wheel_spd;
	volatile uint16_t right_wheel_spd;

	volatile bool speed_is_updated = false;
	WheelSpeedSensor(KBIModule *kbil,KBIModule *kbir,KBISettings::KBI_PTxn left_pin,KBISettings::KBI_PTxn right_pin)
	{
		this->kbil = kbil;
		this->kbir = kbir;
		this->left_pin = left_pin;
		this->right_pin = right_pin;
		this->kbil->EnablePin(left_pin);
		this->kbir->EnablePin(right_pin);
	}


	void GetAndClearCount();//中断函数调用下列函数，其余函数不要使用。
	//有pit中断处理函数将此处获取的值赋给wheel_speed


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
		wheels = new WheelSpeedSensor(new KBIModule(KBIX0,KBI_RISING_HIGH),new KBIModule(KBIX1,KBI_RISING_HIGH),KBI_PTD0,KBI_PTF1);
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
			WheelInit();
		return wheels;
	}
};



#ifdef __cplusplus
extern "C"
{
#endif

volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler();
volatile void __attribute__((interrupt ("IRQ"))) ADC_IRQHandler();
volatile void __attribute__((interrupt ("IRQ"))) KBI0_IRQHandler();
volatile void __attribute__((interrupt ("IRQ"))) KBI1_IRQHandler();

#ifdef __cplusplus
}
#endif






