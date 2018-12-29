/*
 * 定时中断模块驱动
 * 张笑语 12月21日添加
 * pit.cpp
 */
#include "pit.h"
#include "core_cm0plus.h"

using namespace std;

PeriodicInterruptTimer::PeriodicInterruptTimer(PITSettings::Channel channel)
{
    SIM->SCGC |= SIM_SCGC_PIT_MASK;     /*!< enable clock to PIT */
    PIT->MCR |= PIT_MCR_FRZ_MASK; //在debug模式下冻结
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
    PIT->CHANNEL[channel].TFLG  |= PIT_TFLG_TIF_MASK;
    this->channel = channel;
}


PeriodicInterruptTimer*  wPIT::pit_ch0 = nullptr;
