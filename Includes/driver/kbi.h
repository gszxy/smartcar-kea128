/*
 * kbi.h
 *
 *  Created on: 2019年1月10日
 *      Author: 张笑语
 */

#ifndef INCLUDES_DRIVER_KBI_H_
#define INCLUDES_DRIVER_KBI_H_

#include "gpio_pindef.h"
#include "gpio.h"
extern volatile KBI_Type * KBIx[2] ;
/*!
* @brief KBI MODE select enum.
*
*/
namespace KBISettings
{
enum KBI_ModeType
{
    KBI_MODE_EDGE_ONLY = 0,             /*!< select  edge only mode  */
    KBI_MODE_EDGE_LEVEL                 /*!< select  both edge and level  mode */
};

enum KBIn_e
{
    KBIX0 = 0,
    KBIX1
};



 /*!
 * @brief KBI Edge select enum.
 *
 */
enum KBI_Edge
{
    KBI_FALLING_LOW  = 0,     /*!< select falling edge and/or low level  */
    KBI_RISING_HIGH         /*!< select rising edge and/or high level  */
};

enum KBI_PTxn{
KBI_PTA0,
KBI_PTA1,
KBI_PTA2,
KBI_PTA3,
KBI_PTA4,
KBI_PTA5,
KBI_PTA6,
KBI_PTA7,
KBI_PTB0,
KBI_PTB1,
KBI_PTB2,
KBI_PTB3,
KBI_PTB4,
KBI_PTB5,
KBI_PTB6,
KBI_PTB7,
KBI_PTC0,
KBI_PTC1,
KBI_PTC2,
KBI_PTC3,
KBI_PTC4,
KBI_PTC5,
KBI_PTC6,
KBI_PTC7,
KBI_PTD0,
KBI_PTD1,
KBI_PTD2,
KBI_PTD3,
KBI_PTD4,
KBI_PTD5,
KBI_PTD6,
KBI_PTD7,
KBI_PTE0,
KBI_PTE1,
KBI_PTE2,
KBI_PTE3,
KBI_PTE4,
KBI_PTE5,
KBI_PTE6,
KBI_PTE7,
KBI_PTF0,
KBI_PTF1,
KBI_PTF2,
KBI_PTF3,
KBI_PTF4,
KBI_PTF5,
KBI_PTF6,
KBI_PTF7,
KBI_PTG0,
KBI_PTG1,
KBI_PTG2,
KBI_PTG3,
KBI_PTG4,
KBI_PTG5,
KBI_PTG6,
KBI_PTG7,
KBI_PTH0,
KBI_PTH1,
KBI_PTH2,
KBI_PTH3,
KBI_PTH4,
KBI_PTH5,
KBI_PTH6,
KBI_PTH7,
KBI_PTI0,
KBI_PTI1,
KBI_PTI2,
KBI_PTI3,
KBI_PTI4,
KBI_PTI5,
KBI_PTI6,
KBI_PTI7,
};

}

using namespace KBISettings;

class KBIModule
{
	KBIn_e module;
	KBI_Edge edge;
public:
	KBIModule(KBISettings::KBIn_e kbix,KBISettings::KBI_Edge edge_select) ;
	inline void DisablePin(KBISettings::KBI_PTxn ptxn)
	{
		KBIx[module]->PE &= ~(1 << PTn(ptxn)) ;
	}
	inline void EnablePin(KBISettings::KBI_PTxn ptxn)
	{
		KBIx[module]->SC |= KBI_SC_RSTKBSP_MASK ;    //清除中断标志位
		KBIx[module]->PE |= (1 << PTn(ptxn)) ;
		if(module == KBIX0 )
				  {
				        FGPIOA->PIDR  &= ~(1<<PTn(ptxn));              /* enable GPIO input */
				        FGPIOA->PDDR  &= ~(1<<PTn(ptxn));
				        if(edge)
				        	PORT->PUE0  &= ~(1<<PTn(ptxn));
				        else
				        	PORT->PUE0  |= (1<<PTn(ptxn));

				    }
				else if (module == KBIX1)
				    {
				        FGPIOB->PIDR  &= ~(1<<PTn(ptxn));              /* enable GPIO input */
				        FGPIOB->PDDR  &= ~(1<<PTn(ptxn));
				        if(edge)
				        	PORT->PUE1  &= ~(1<<PTn(ptxn));
				        else
				        	PORT->PUE1  |= (1<<PTn(ptxn));

				    }
				  if(edge)
				  {
				    KBIx[module]->ES  |= (1<< PTn(ptxn));
				  }
				  else
				  {
				    KBIx[module]->ES  &= ~(1<< PTn(ptxn));
				  }
				  KBIx[module]->PE |= (1 << PTn(ptxn)) ;
	}
};






#endif /* INCLUDES_DRIVER_KBI_H_ */
