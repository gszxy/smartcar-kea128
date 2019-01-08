/*
 * motor.cpp
 *
 *  Created on: 2018年12月24日
 *      Author: 张笑语
 */
#include "motor.h"

Motor* MotorSingletons::left_motor = nullptr;
Motor* MotorSingletons::right_motor = nullptr;
uint8_t Motor::cp = 20000;
uint8_t Motor::ci = 1000;
uint8_t Motor::cd = 3000;
void Motor::DoPIDSpdControl(int16_t measured_spd)
{//增量式PID控制
	int16_t error = this->closed_loop_speed - measured_spd;
	for(uint8_t i=1; i<6;i++)
		this->historical_errs[i] = this->historical_errs[i-1];
	this->historical_errs[0] = error;
	this->historical_errs_sum += error;
	int16_t diff = (historical_errs[0]+historical_errs[1]+historical_errs[2]) - (historical_errs[3]+historical_errs[4]+historical_errs[5]);
	//逐差法求微分项
	int32_t output = error * cp + historical_errs_sum * ci + diff * cd;
	int16_t final = this->duty_cyc + output/1000;
	if(final < 0)//保护性截断
		final = 0;
	else if(final > 10000)
		final = 10000;
	duty_cyc = static_cats<uint16_t>(final);
	ftc->SetDutyCycle(duty_cyc);
}
