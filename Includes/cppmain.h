/*
 * cppmain.h
 *
 *  Created on: 2018年12月15日
 *      Author: 张笑语
 */

#ifndef INCLUDES_CPPMAIN_H_
#define INCLUDES_CPPMAIN_H_


#ifdef __cplusplus
extern "C"{
#endif

extern int _cppmain();

/*............下面是中断处理函数............*/
volatile void __attribute__ ((noinline)) UART0_IRQHandler();
volatile void __attribute__ ((noinline)) UART1_IRQHandler();
volatile void __attribute__ ((noinline)) UART2_IRQHandler();
#ifdef __cplusplus
}
#endif


#endif /* INCLUDES_CPPMAIN_H_ */
