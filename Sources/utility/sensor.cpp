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
#include "kbi.h"
#include "gpio.h"
using namespace std;

InductorSensor *SensorSingletons::inductors = nullptr;
WheelSpeedSensor *SensorSingletons::wheels = nullptr;

//本文件内的全局变量
volatile uint16_t encoder_count[2] = {0,0};


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



void WheelSpeedSensor::GetAndClearCount()
{
	this->left_wheel_spd = encoder_count[0];
	this->right_wheel_spd = encoder_count[1];
	encoder_count[0]=encoder_count[1]=0;
	this->speed_is_updated = true;
}



volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler()
{//定时中断，开始AD转换和数据更新

	static uint8_t counter = 0;//计数器，用于控制车轮测速。每2个中断周期测量一侧轮胎的速度
	counter++;
	if(counter == 1)
	{
		SensorSingletons::GetWheelSpeedSensor()->GetAndClearCount();
	}
	else if(counter == 10)
		counter = 0;


	SensorSingletons::GetInductorSensor()->StartConvert();

	PIT->CHANNEL[0].TFLG  |= PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[1].TFLG  |= PIT_TFLG_TIF_MASK;
	//清标志位，否则中断会不断触发
}

volatile void __attribute__((interrupt ("IRQ"))) KBI0_IRQHandler()
{
		++encoder_count[0];
	KBIx[KBIX0]->SC |= (KBI_SC_KBACK_MASK | KBI_SC_RSTKBSP_MASK | KBI_SC_KBIE_MASK | KBI_SC_KBSPEN_MASK);
	KBIx[KBIX0]->PE |= (1 << PTn(SensorSingletons::GetWheelSpeedSensor()->left_pin));

}
volatile void __attribute__((interrupt ("IRQ"))) KBI1_IRQHandler()
{
	++encoder_count[1];
	KBIx[KBIX1]->SC |= (KBI_SC_KBACK_MASK | KBI_SC_RSTKBSP_MASK | KBI_SC_KBIE_MASK | KBI_SC_KBSPEN_MASK);
	KBIx[KBIX1]->PE |= (1 << PTn(SensorSingletons::GetWheelSpeedSensor()->right_pin));
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
