/*
 * vehicle_settings.h
 *
 *  Created on: 2018年12月18日
 *      Author: 张笑语
 *      智能车程序控制类设置。例如默认速度等等
 */

#ifndef INCLUDES_VEHICLE_SETTINGS_H_
#define INCLUDES_VEHICLE_SETTINGS_H_

#include <cstdint>

struct PIDParams
{
	uint16_t p;
	uint16_t i;
	uint16_t d;
};

static class VehicleSettings
{
public:
	static uint16_t default_speed;
	static uint16_t steer_mid_freq;//舵机中位对应的PWM控制信号频率
	static uint16_t steer_max_turn_freq_change; //舵机从中位向一侧走到尽头所需要的频率变化量
	static PIDParams steer_pid_params; //舵机PID控制系数
	static PIDParams motor_pid_params; //电机PID控制系数

	static inline void GetVehicleSettings()
	{

	}
};




#endif /* INCLUDES_VEHICLE_SETTINGS_H_ */
