/*
 * steer.h
 *
 *  Created on: 2019年1月8日
 *      Author: 张笑语
 */

#ifndef INCLUDES_UTILITY_STEER_H_
#define INCLUDES_UTILITY_STEER_H_
#include <cstdint>
#include "ftm.h"

#define STEER_PWM_FREQ             329       //舵机使用的PWM频率
#define STEER_ZERO_YAW_DUTY_CYC    5000      //舵机零偏航时的占空比，单位万分之一
#define STEER_MAX_YAW_DUTY_CHANGE  500       //舵机最大偏航时的占空比变化量

class Steer
{
private:
	FlexTimerModule  *ftm;
	FlexTimerChannel *ftc;
public:
	Steer(FlexTimerModule* module, FlexTimerChannel* channel)
	{
		ftm = module;
		ftc = channel;
		ftm ->SetFrequency(STEER_PWM_FREQ);
		ftc ->SetDutyCycle(STEER_ZERO_YAW_DUTY_CYC);
		ftm->EnableBusClock();
		ftc->EnablePWMOutput();
	}
	inline void SetYaw(int16_t percentage)//传入一个-1000至1000的值，按比例调整舵机方向
	{
		if(percentage > 1000)
			percentage = 1000;
		else if(percentage < -1000)
			percentage = -1000;
		uint16_t pwm_duty_cyc = STEER_ZERO_YAW_DUTY_CYC + percentage * STEER_MAX_YAW_DUTY_CHANGE / 1000 ;
		ftc->SetDutyCycle(pwm_duty_cyc);
	}
};

class SteerSingleton
{
private:
	static Steer *stptr;
	static inline void Init()
	{
		FlexTimerModule *ftm = new FlexTimerModule(FTMSettings::mFTM2);
		FlexTimerChannel *ftc = new FlexTimerChannel(ftm,FTMSettings::FTM_CH0,FTMSettings::remap_first);
		stptr = new Steer(ftm,ftc);
	}
public:
	static inline Steer* GetSteerObj()
	{
		if(stptr == nullptr)
			Init();
		return stptr;
	}
};


#endif /* INCLUDES_UTILITY_STEER_H_ */
