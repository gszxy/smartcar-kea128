#pragma once


#include <cstdint>
#include "adc.h"
#include "pit.h"






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

extern InductorSensor *g_sensor;

#ifdef __cplusplus
extern "C"
{
#endif

volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler();
volatile void __attribute__((interrupt ("IRQ"))) ADC_IRQHandler();

#ifdef __cplusplus
}
#endif






