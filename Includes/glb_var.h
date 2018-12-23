/*
 * glb_variables.h
 *
 *  Created on: 2018年12月16日
 *      Author: 张笑语
 *
 *      程序全局变量定义文件
 *      重要提示：切勿随意使用全局变量。程序原则上只允许使用以下几种全局符号
 *      1.硬件抽象层宏定义和效果功能均类似的全局变量。不需要在此文件中定义
 *      2.函数和类型定义（显然）
 *      3.程序中各部分共用的公共对象，如确实属于公共环境，请定义在此文件中，并在__glb_variables_init()中初始化。例如此处有的串口、电机控制器等等
 *      4.枚举类型请包含在自定义命名空间中，防止污染全局和std命名空间
 *      能用参数或成员变量传递的信息不得定义为全局变量。不要给自己和同学增加无谓的工作量，切记！
 */

#ifndef INCLUDES_GLB_VAS_H_
#define INCLUDES_GLB_VAS_H_

#include "communicate.h"
#include "adc.h"


extern UARTCommunicator *g_uartc;

extern ADCModule *g_test_adc;


#endif /* INCLUDES_GLB_VAS_H_ */
