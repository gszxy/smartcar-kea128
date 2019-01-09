/*
 * sensor.cpp
 *
 *  Created on: 2018年12月21日
 *      Author: 张笑语
 */

#include "sensor.h"
#include "adc.h"
#include "glb_var.h"
#include "SKEAZ1284.h"
#include "ftm.h"

using namespace std;

InductorSensor *SensorSingletons::inductors = nullptr;
WheelSpeedSensor *SensorSingletons::wheels = nullptr;


InductorSensor::InductorSensor()
{
	adcm = new ADCModule[4]{ADCModule(ADC_CHANNEL_AD0,ADC_12BIT),ADCModule(ADC_CHANNEL_AD1,ADC_12BIT),
		                    ADCModule(ADC_CHANNEL_AD2,ADC_12BIT),ADCModule(ADC_CHANNEL_AD3,ADC_12BIT)};
	//A0 A1 A6 A7
}

InductorSensor::~InductorSensor()
{
	delete[] adcm;
}

void InductorSensor::StartConvert()
{
	for(int i=0;i<4;i++)
	{
		adcm[i].StartConversion(3);
		adcm[i].WaitUntilFinished();
		uint16_t dt = adcm[i].TryFetchResult();
		this->values[i] = dt;
	}
	this->flag_data_updated = true;

}







volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler()
{//定时中断，开始AD转换和数据更新

	static uint8_t counter = 0;//计数器，用于控制车轮测速。每2个中断周期测量一侧轮胎的速度
	counter++;
	if(counter == 1)
	{
		SensorSingletons::GetWheelSpeedSensor()->GetAndClearCount();
		SensorSingletons::GetWheelSpeedSensor()->LeftStart();
	}
	else if(counter == 3)
	{
		SensorSingletons::GetWheelSpeedSensor()->GetAndClearCount();
		SensorSingletons::GetWheelSpeedSensor()->RightStart();
	}
	else if(counter == 4)
		counter = 0;


	SensorSingletons::GetInductorSensor()->StartConvert();

	PIT->CHANNEL[0].TFLG  |= PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[1].TFLG  |= PIT_TFLG_TIF_MASK;
	//清标志位，否则中断会不断触发
}
/*
volatile void __attribute__((interrupt ("IRQ"))) ADC_IRQHandler()
{
	uint8_t finished_adc_id =  g_sensor->current_adc_m;
	g_sensor->
	if(finished_adc_id <= 2)
	{
		g_sensor->adcm[++finished_adc_id].StartConversion(7);
	}
	else//=3
	{
		g_sensor->sys_has_ongoing_convert = false;
		g_sensor->inductor_data->flag_data_updated = true; //指示数据已经更新完成
	}

}
*/
