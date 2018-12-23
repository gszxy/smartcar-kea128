/*
 * 定时中断模块驱动
 * 张笑语 12月21日添加
 * pit.cpp
 */
#include "pit.h"
#include "core_cm0plus.h"

using namespace std;

PeriodicInterruptTimer::PeriodicInterruptTimer(PITsettings::Channel channel)
{
    SIM->SCGC |= SIM_SCGC_PIT_MASK;     /*!< enable clock to PIT */

    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    PIT->CHANNEL[channel].LDVAL = 0xFFFFFFFF ;//用户没有设置之前，默认为最大中断间隔
    PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
    PIT->CHANNEL[channel].TFLG  |= PIT_TFLG_TIF_MASK;
    this->channel = channel;
}
