/*
 * motor.h
 *
 *  Created on: 2018年12月24日
 *      Author: 张笑语
 */

#ifndef INCLUDES_UTILITY_MOTOR_H_
#define INCLUDES_UTILITY_MOTOR_H_


#include <cstdint>
#include "ftm.h"
#include "gpio.h"

class Motor
{
	GPIOModule *gpio_c;  //控制正反转的gpio模块,暂不使用
	FlexTimerModule *ftm;//ftm模块
	FlexTimerChannel *ftc;//ftm输出通道
	uint16_t duty_cyc = 0;
	uint16_t closed_loop_speed = 0;
	static uint8_t cp;
	static uint8_t ci;
	static uint8_t cd;//闭环控制的pid参数。默认值在对应cpp文件内
	int16_t historical_errs[6];//传感器测量到的过去几个速度值的误差，用于pid
	int32_t historical_errs_sum = 0;//误差和，用于积分项
public:
	Motor(FlexTimerModule *module,FlexTimerChannel *channel)
	{
		ftm = module;
		ftc = channel;
		ftm->SetFrequency(20000);
		for(uint8_t i=0;i<6;i++)
			historical_errs[i] = 0;
	}
	inline void SetMotorOpenLoopSpeed(uint16_t ftm_duty_cycle)
	{
		duty_cyc = ftm_duty_cycle;
		ftc->SetDutyCycle(duty_cyc);
	}
	inline void SetMotorClosedLoopSpeed(uint16_t spd)
	{
		closed_loop_speed = spd;
	}
	inline void Run()
	{
		ftc->EnablePWMOutput();
	}
	inline void Halt()
	{
		ftc->DisablePWMOutput();
	}
	void DoPIDSpdControl(int16_t measured_spd);
	~Motor()
	{

	}
	static inline void SetPIDParams(uint8_t p,uint8_t i,uint8_t d)//单位：千分之一
	{
		cp=p;
		ci=i;
		cd=d;
	}
};

class MotorSingletons  //对电机类进行单例模式包装
{
private:
	static Motor *left_motor;
	static Motor *right_motor ;
	static inline void MotorInit()
	{
		auto ftm = new FlexTimerModule(FTMSettings::mFTM0);
		auto ftcl = new FlexTimerChannel(ftm,FTMSettings::FTM_CH0,FTMSettings::remap_first);
		auto ftcr = new FlexTimerChannel(ftm,FTMSettings::FTM_CH1,FTMSettings::remap_first);
		right_motor = new Motor(ftm,ftcr);
		right_motor ->SetMotorOpenLoopSpeed(6000);//默认速度
		right_motor->SetMotorClosedLoopSpeed(50);
		left_motor = new Motor(ftm,ftcl);
		left_motor->SetMotorOpenLoopSpeed(6000);//默认速度
		left_motor->SetMotorClosedLoopSpeed(50);
	}
public:
	static inline Motor* GetLeftMotorObj()
	{
		if(left_motor == nullptr)
			MotorInit();
		return left_motor;
	}
	static inline Motor* GetRightMotorObj()
	{
		if(right_motor == nullptr)
			MotorInit();
		return right_motor;
	}


};
#endif /* INCLUDES_UTILITY_MOTOR_H_ */
