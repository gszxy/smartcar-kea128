#include "adc.h"
#include "SKEAZ1284.h"
#include <cstdint>
#include <cassert>
using namespace std;
/*
 * 张笑语 2018年12月8r日 编写
 */
/*
ADC_CHANNEL_AD0 -------------- A0   
ADC_CHANNEL_AD1 -------------- A1
ADC_CHANNEL_AD2 -------------- A6 
ADC_CHANNEL_AD3 -------------- A7
ADC_CHANNEL_AD4 -------------- B0
ADC_CHANNEL_AD5 -------------- B1  
ADC_CHANNEL_AD6 -------------- B2
ADC_CHANNEL_AD7 -------------- B3
ADC_CHANNEL_AD8 -------------- C0
ADC_CHANNEL_AD9 -------------- C1 
ADC_CHANNEL_AD10 ------------- C2
ADC_CHANNEL_AD11 ------------- C3
ADC_CHANNEL_AD12 ------------- F4
ADC_CHANNEL_AD13 ------------- F5 
ADC_CHANNEL_AD14 ------------- F6
ADC_CHANNEL_AD15 ------------- F7

UARTR0:PTA3 TX ,PTA2 RX
UARTR1:PTF3 TX ,PTF2 RX
UARTR2:PTI1 TX ,PTI0 RX
*/
uint32_t ADCModule::chn_occupation_flag = 0;

int8_t ADCModule::StartConversion(uint8_t count)
{//count:连续转换count次取平均值。只允许1-8范围内的值
	if(ADCModule::GetIsConversionOngoing())//如果存在正在进行的转换没有完成
		return -1;//返回错误码-1
//	if(ADC->SC1 & ADC_SC1_COCO_MASK )//如果已经有完成的转换，但是没有被读取
//		return -2;//返回错误码-2
	this->channel_active = true;//当前通道活跃
	this->active_convert_count = count;

	ADC->SC3  = (0
		|ADC_SC3_ADICLK(BUS_CLOCK)   //选择系统时钟
		|ADC_SC3_MODE(this->bit)      //AD位数
		//|ADC_SC3_ADLSMP_MASK         //低功耗采集
		|ADC_SC3_ADIV(ADC_ADIV_1)      //分频为1
		//|ADC_SC3_ADLPC_MASK            //长步长时间
	) ;
	ADC->SC2  = (0
	// |ADC_SC2_ADTRG_MASK             //1硬件触发,0软件触发
	//  |ADC_SC2_ACFE_MASK            //采集比较
	                    //  |ADC_SC2_ACFGT_MASK           //大于比较值，比较触发
	                    ) ;
	ADC->APCTL1 = ADC_APCTL1_ADPC(1<< this->channel) ;
    ADC->SC4 =(0
    	|ADC_SC4_AFDEP(count-1) //FIFO深度
    );
    //向FIFO队列循环发送count次ADC请求，FIFO和此处操作见英文版用户手册344页或中文版326页
    for(int8_t i=1;i<=count;i++)
	{
    	ADC->SC1  = (0
    			|ADC_SC1_ADCH(this->channel)             //选择采集通道
				// |ADC_SC1_ADCO_MASK            //连续采集
			    //|ADC_SC1_AIEN_MASK           //中断
    	) ;
	}

    return 0;
}

ADCModule::ADCModule(ADCHn channel/*ADC转换的通道*/,ADC_nbit bit/*ADC转换的位数，支持8，10，12位*/)//构造函数
{
	//存参数
	this->channel = channel;
	this->bit = bit;
	//可以使用，置标志
	uint32_t mask = (1<<(int)channel);
	ADCModule::chn_occupation_flag |= mask;

	SIM->SCGC |= SIM_SCGC_ADC_MASK;   //为ADC提供时钟源
}

ADCModule::~ADCModule()//析构函数
{
	//移除通道占用标志位
	uint32_t mask = (1<<(int)channel);
	this->chn_occupation_flag ^= mask; //按位异或然后赋值，将对应标志位清零，其余不变
}

uint16_t ADCModule::TryFetchResult()
{
	//if(!this->channel_active)//通道未激活，说明当前存放的结果要么不存在，要么不属于此通道
	//	return 0xFFFF;//正常转换最多12位，这个数字不可能出现。作错误码。
	//if(!(ADC->SC1 & ADC_SC1_COCO_MASK))//转换未完成
	//	return 0xFFFE;
	int16_t sum = 0;

	for(int8_t count = this->active_convert_count; count>=1; count--)
	{
		sum += ADC->R;
	}
	uint16_t final = sum / this->active_convert_count;
	return final ;//返回平均值
}





