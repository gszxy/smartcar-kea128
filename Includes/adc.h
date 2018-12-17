/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file adc.h
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief header file for ADC module utilities (ADC). 
*
*张笑语  2018年12月8日修改  增加面向对象功能
*此头文件只支持c++
*******************************************************************************
*
* provide APIs for accessing ADC module (ADC)
******************************************************************************/

#include <cstdint>
#include "SKEAZ1284.h"
using namespace std;

#ifndef ADC_H_
#define ADC_H_

/******************************************************************************
* Macros
******************************************************************************/
/******************************************************************************
*define ADC refernce voltage 
*
*//*! @addtogroup adc_ref_list
* @{
*******************************************************************************/

#define ADC_VREF_VREFH                 0x00			/*!< ADC reference voltage is VREFH*/
#define ADC_VREF_VDDA                  0x01			/*!< ADC reference voltage is VDDA*/

/*! @} End of adc_ref_list                                                    						*/

/******************************************************************************
* define ADC clock source 
*
*//*! @addtogroup adc_clock_source_list
* @{
*******************************************************************************/

#define BUS_CLOCK		0x00	/*!< ADC clock source is bus clock*/
#define BUS_CLOCK_DIVIDE_2	0x01	/*!< ADC clock source is bus clock devided by 2*/
#define ALTCLK		0x02	/*!< ADC clock source is alternative clcok*/
#define ADACK		0x03	/*!< ADC clock source is asynchronous clock*/
/*! @} End of adc_clock_source_list                                          						*/


/******************************************************************************
* define ADC divider 
*
*//*! @addtogroup adc_clock_divider_list
* @{
*******************************************************************************/

#define ADC_ADIV_1	       0x00/*!< ADC clock divide by 1*/
#define ADC_ADIV_2	       0x01/*!< ADC clock divide by 2*/
#define ADC_ADIV_4         0x02/*!< ADC clock divide by 4*/
#define ADC_ADIV_8	       0x03/*!< ADC clock divide by 8*/
/*! @} End of adc_clock_divider_list                                          						*/

/******************************************************************************
* define ADC mode 
*
*//*! @addtogroup adc_mode_list
* @{
*******************************************************************************/
enum ADC_nbit
{
    ADC_8BIT   = 0x00,
    ADC_10BIT  = 0x01,
    ADC_12BIT  = 0x02,
};
/*! @} End of adc_mode_list                                               						*/

/******************************************************************************
* define ADC channel
*
*//*! @addtogroup adc_channel_list
* @{
*******************************************************************************/
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
*/

enum ADCHn
{
  ADC_CHANNEL_AD0                 =0x0, /*!< ADC input channel 0 */
  ADC_CHANNEL_AD1                 =0x1,/*!< ADC input channel 1 */
  ADC_CHANNEL_AD2                 =0x2, /*!< ADC input channel 2 */
  ADC_CHANNEL_AD3                 =0x3, /*!< ADC input channel 3 */
  ADC_CHANNEL_AD4                 =0x4, /*!< ADC input channel 4 */
  ADC_CHANNEL_AD5                 =0x5, /*!< ADC input channel 5 */
  ADC_CHANNEL_AD6                 =0x6, /*!< ADC input channel 6 */
  ADC_CHANNEL_AD7                 =0x7, /*!< ADC input channel 7 */
  ADC_CHANNEL_AD8                 =0x8, /*!< ADC input channel 8 */
  ADC_CHANNEL_AD9                 =0x9, /*!< ADC input channel 9 */
  ADC_CHANNEL_AD10                =0xa, /*!< ADC input channel 10 */
  ADC_CHANNEL_AD11                =0xb, /*!< ADC input channel 11 */
  ADC_CHANNEL_AD12                =0xc, /*!< ADC input channel 12 */
  ADC_CHANNEL_AD13                =0xd, /*!< ADC input channel 13 */
  ADC_CHANNEL_AD14                =0xe, /*!< ADC input channel 14 */
  ADC_CHANNEL_AD15                =0xf, /*!< ADC input channel 15 */
  ADC_CHANNEL_AD18_VSS            =0x12, /*!< ADC input channel VSS */ 
  ADC_CHANNEL_AD22_TEMPSENSOR     =0x16, /*!< ADC input channel internal temperature sensor */
  ADC_CHANNEL_AD23_BANDGAP        =0x17, /*!< ADC input channel bandgap */ 
  ADC_CHANNEL_AD29_VREFH          =0x1D, /*!< ADC input channel Vrefh */ 
  ADC_CHANNEL_AD30_VREFL          =0x1E, /*!< ADC input channel Vrefl */ 
  ADC_CHANNEL_DISABLE             =0x1F, /*!< ADC disable */
};
/*! @} End of adc_channel_list                                               						*/





/******************************************************************************
* Objective A-D conversion classes
* 张笑语 2018年12月8日添加
******************************************************************************/

class ADCModule
{
	//todo:支持中断模式ADC转换
private:
	static uint32_t chn_occupation_flag;
	//15个ADC通道+电源和温度等额外通道，用这个数的每一位记载每一个channel是否已经被初始化

	ADCHn channel;
	ADC_nbit bit;
	bool channel_active = false;//该adc通道是否正在占用转换。防止转换结果被其它通道错误地读取
	int8_t active_convert_count = 0;//保存当前正在进行转换的连续转换次数
public:
	static inline bool IsModuleInited(ADCHn _channel)
	{
		uint32_t mask = (1<<(int)_channel);
		return ADCModule::chn_occupation_flag & mask ;
	}//返回ADC通道是否已经被初始化
    static inline bool GetIsConversionOngoing()
	{
    	return ADC->SC2 | ADC_SC2_ADACT_MASK;
	}
    inline void WaitUntilFinished()
    {
    	while ((ADC->SC1 & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);
    }

	ADCModule(ADCHn channel/*ADC转换的通道*/,ADC_nbit bit/*ADC转换的位数，支持8，10，12位*/);
    int8_t StartCoversion(int8_t count /*count的大小支持1-8*/);
    uint16_t TryFetchResult();
    ~ADCModule();

};

namespace //匿名命名空间，禁止从外部访问
{

}

#endif /* ADC_H_ */
