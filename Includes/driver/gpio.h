/*
 * gpio.h
 *
 *  Created on: 2018年12月15日
 *      Author:
 */

#ifndef INCLUDES_GPIO_H_
#define INCLUDES_GPIO_H_

#include <cstdint>
#include "gpio_pindef.h"
#include "SKEAZ1284.h"

extern volatile GPIO_Type *GPIOx[];


#define PTX(PTxn)           ((PTxn)>>5)              //PORTX PORTA PORTB
#define PTn(PTxn)           ((PTxn)&0x1f)           //PIN   0~31
#define GPIOX_BASE(PTxn)     GPIOx[PTX(PTxn)]       //GPIOx模块的地址

#define GPIO_SET(PORTx,n,x)           GPIO_SET_##x((PORTx),(n))                                 //设置输出电平x，x为0或1   例如 GPIO_SET(PORTA,1,1)  PA1输出高电平
#define GPIO_TURN(PORTx,n)           (GPIO_PDOR_REG(GPIOx[(PORTx)]) ^=  (1<<(n)))               //翻转输出电平
#define GPIO_GET(PORTx,n)            ((GPIO_PDIR_REG(GPIOx[(PORTx)])>>(n))&0x1)                 //读取引脚输入状态
//GPIO  1位操作
#define GPIO_SET_1bit(PORTx,n,data)   GPIO_PDOR_REG(GPIOx[(PORTx)])=(( GPIO_PDOR_REG(GPIOx[(PORTx)])& ~(0x1<<(n)) )|(((data)&0x01)<<(n)))   //写1位数据（n为最低位引脚号）
#define GPIO_DDR_1bit(PORTx,n,ddr)    GPIO_PDDR_REG(GPIOx[(PORTx)])=(( GPIO_PDDR_REG(GPIOx[(PORTx)])& ~(0x1<<(n)) )|(((ddr)&0x01)<<(n)))    //设置1位输入输出方向（n为最低位引脚号）
#define GPIO_GET_1bit(PORTx,n)        (( GPIO_PDIR_REG(GPIOx[(PORTx)])>>(n) ) & 0x1)                                                        //读1位数据（n为最低位引脚号）
//GPIO  8位操作
#define GPIO_SET_8bit(PORTx,n,data)   GPIO_PDOR_REG(GPIOx[(PORTx)])=(( GPIO_PDOR_REG(GPIOx[(PORTx)])& ~(0xff<<(n)) )|(((data)&0xff)<<(n)))  //写8位数据（n为最低位引脚号）
#define GPIO_DDR_8bit(PORTx,n,ddr)    GPIO_PDDR_REG(GPIOx[(PORTx)])=(( GPIO_PDDR_REG(GPIOx[(PORTx)])& ~(0xff<<(n)) )|(((ddr)&0x0ff)<<(n)))  //设置8位输入输出方向（n为最低位引脚号）
#define GPIO_GET_8bit(PORTx,n)        (( GPIO_PDIR_REG(GPIOx[(PORTx)])>>(n) ) & 0xff)                                                       //读8位数据（n为最低位引脚号）
/****************************内部使用，用户不需要关心****************************/
#define GPIO_SET_1(PORTx,n)          GPIO_PDOR_REG(GPIOx[(PORTx)]) |=  (1<<(n))      //设置输出为高电平        例如：GPIO_SET_H(PORTA,1)   PA1输出高电平
#define GPIO_SET_0(PORTx,n)          GPIO_PDOR_REG(GPIOx[(PORTx)]) &= ~(1<<(n))      //设置输出为低电平        例如：GPIO_SET_L(PORTA,1)   PA1输出低电平



class GPIOModule
{//TODO:添加用于读取引脚输入输出模式、确认某引脚是否已经被占用的功能
private:
	PTxn port;
public:
	GPIOModule(PTxn port,bool direction_is_output);
	~GPIOModule();
	inline void SetState(bool state)
	{
		//需要指出，这个if else并不是多余的
		//因为GPIO_SET这个宏只能接受0或者1，拿其它数去做位运算的话就会干扰其它引脚上的电平状态
		if(state)
		{
			GPIO_SET(PTX(port), PTn(port), 1);
		}
		else
		{
		    GPIO_SET(PTX(port), PTn(port), 0);
		}
	}
	inline void ToggleState()//翻转状态
	{
		GPIO_TURN(PTX(port), PTn(port));
	}
	inline bool GetState()//仅在输入模式下可用
	{
		return GPIO_GET(PTX(port),PTn(port));
	}
	inline void SetDirection(bool direction_is_output)
	{
		if (direction_is_output)   //输出
		{
		    GPIO_PDDR_REG(GPIOX_BASE(this->port)) |= (1 << PTn(static_cast<uint8_t>(this->port)));  //设置端口方向为输出
		}
		else
		{
			GPIO_PDDR_REG(GPIOX_BASE(this->port)) &= ~(1 << PTn(this->port));  //设置端口方向为输入
		}

	}
};

//TODO:如果有需要，添加按8位操作的类

#endif /* INCLUDES_GPIO_H_ */
