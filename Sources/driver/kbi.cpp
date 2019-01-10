/*
 * kbi.cpp
 *
 *  Created on: 2019年1月10日
 *      Author: 张笑语
 */

#include "kbi.h"
#include "gpio.h"
using namespace KBISettings;

volatile KBI_Type * KBIx[2] = { KBI0,KBI1} ;

KBIModule::KBIModule(KBIn_e kbix,KBI_Edge edge_select)
{
	this->module = kbix;
	this->edge = edge_select;
	  if(kbix == KBIX0 )
	    {
	        SIM->SCGC   |= SIM_SCGC_KBI0_MASK;             /* enable clock to KBI0 */
	        KBIx[kbix]->SC  &= ~KBI_SC_KBIE_MASK ;
	    }
	    else if (kbix == KBIX1)
	    {
	        SIM->SCGC   |= SIM_SCGC_KBI1_MASK;             /* enable clock to KBI1 */
	        KBIx[kbix]->SC  &= ~KBI_SC_KBIE_MASK ;
	    }



	  PORT->IOFLT0 |= PORT_IOFLT0_FLTDIV1(0) ;
	  PORT->IOFLT0 |= PORT_IOFLT0_FLTKBI0(1) ;
	  KBIx[kbix]->SC  = (0
	                     //|KBI_SC_KBMOD_MASK           //跳变沿和水平中断
	                     |KBI_SC_KBIE_MASK              //中断使能
	                     |KBI_SC_KBACK_MASK             //
	                     |KBI_SC_KBSPEN_MASK          //锁存KBxSP
	                     |KBI_SC_RSTKBSP_MASK           //清除KBIxSP
	                      );

	  if(kbix == KBIX0 )
	   {
	     NVIC_EnableIRQ(KBI0_IRQn);
	   }
	  else if (kbix == KBIX1)
	    {
	      NVIC_EnableIRQ(KBI1_IRQn);
	    }
}


