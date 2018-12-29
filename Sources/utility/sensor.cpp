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

using namespace std;

InductorSensor *g_sensor = nullptr;


InductorSensor::InductorSensor()
{
	adcm = new ADCModule[3]{ADCModule(ADC_CHANNEL_AD0,ADC_12BIT),ADCModule(ADC_CHANNEL_AD1,ADC_12BIT),ADCModule(ADC_CHANNEL_AD2,ADC_12BIT)};
}

InductorSensor::~InductorSensor()
{
	delete[] adcm;
}

void InductorSensor::StartConvert()
{
	for(int i=0;i<3;i++)
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
	PIT->CHANNEL[0].TFLG  |= PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[1].TFLG  |= PIT_TFLG_TIF_MASK;
	g_sensor->StartConvert();
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
