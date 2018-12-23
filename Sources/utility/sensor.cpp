/*
 * sensor.cpp
 *
 *  Created on: 2018年12月21日
 *      Author: 张笑语
 */

#include "sensor.h"
#include "adc.h"

using namespace std;

InductorSensor *g_sensor;


InductorSensor::InductorSensor()
{
	adcm = new ADCModule[3]{ADCModule(ADC_CHANNEL_AD0,ADC_12BIT),ADCModule(ADC_CHANNEL_AD1,ADC_12BIT),ADCModule(ADC_CHANNEL_AD0,ADC_12BIT)};
	for(int i=0;i<3;i++)
	{
		adcm[i].EnableIntrOnCovertFinished();//开转换完成中断
	}
	inductor_data = new InductorData();

}

InductorSensor::~InductorSensor()
{
	delete[] adcm;
	delete inductor_data;
}

void InductorSensor::StartConvert()
{
	if(sys_has_ongoing_convert)
		return;
	adcm[0].StartConversion(7);
	//开始第一个转换。之后的就交给中断处理函数了
	current_adc_m = 0;
	sys_has_ongoing_convert = true;
}

volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler()
{//定时中断，开始AD转换和数据更新
	g_sensor->StartConvert();
}

volatile void __attribute__((interrupt ("IRQ"))) ADC_IRQHandler()
{
	uint8_t finished_adc_id =  g_sensor->current_adc_m;
	g_sensor->inductor_data->values[finished_adc_id] =  g_sensor->adcm[finished_adc_id].TryFetchResult();
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
