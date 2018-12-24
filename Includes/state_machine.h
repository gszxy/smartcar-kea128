/*
 * state_machine.h
 *
 *  Created on: 2018年12月15日
 *      Author: 张笑语
 * 有限状态机实现文件
 */
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "uart.h"
#include "glb_var.h"
namespace cmd//避免污染全局命名空间
{
	enum Event  //可能导致状态机状态发生变化的一系列事件
	{

		/*..........运行事件..........*/
		run_start,
		run_entry_curve,
		run_entry_upslope,
		run_entry_downslope,
		run_entry_ring,
		run_leave_special,
		run_finish,
		/*..........异常事件..........*/
		exp_no_signal,
		exp_speed_abnormal,
		exp_out_of_track,
		exp_blocked,
		/*..........其它事件..........*/
		noevent //本周期内没有发生任何事件
	};

	enum Command //由上位机发送的指令
	{
		nocommand,
		/*..........停止指令..........*/
		stop_force, //强制停止
		stop_ready,
		/*..........启动指令..........*/
		start,
		/*..........测试指令..........*/
		test_uart,
		test_adc,
		test_ftm,
		test_gpio,
		/*..........复位指令..........*/
		reset
	};
}


class State
{

public:
	virtual State* HandleCommand(uint8_t command) {}
	virtual State* HandleInput() {}
	virtual void StateChangeOp()  //从其它状态切换到当前状态时被状态机调用
	{
	}
	virtual void StateRemainOp()
	{
	}
	virtual void StateExitOp()
	{
		char msg[] = "Exit..\n";
		g_uartc->SendString(msg,strlen(msg));
	}

};


class TestADConverterState :public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual State* HandleInput(){}
	virtual void StateChangeOp();  //从其它状态切换到当前状态时被状态机调用

	virtual void StateRemainOp();
	virtual void StateExitOp();

};

class TestUARTState :public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual State* HandleInput();
	virtual void StateChangeOp();
};
class IdleState :public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual State* HandleInput();
	virtual void StateChangeOp(){}
};


class TestFTMState : public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual void StateChangeOp();
};


/*
 * 车辆运行状态组：类实现放在vehicle.cpp中
 */

class RunningState : public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual State* HandleInput(){}
	virtual void StateChangeOp()  //从其它状态切换到当前状态时被状态机调用
	{
		char msg[] = "running...\n";
		g_uartc->SendString(msg,strlen(msg));
	}
	virtual void StateRemainOp();
	virtual cmd::Event AnalyseSensorData(){}//通过传感器数据确定设备是否出现特殊时间，如偏离赛道等
	                                       //提线算法由控制器完成，与状态机的此方法无关
	virtual void StateExitOp();
};
//TODO:实现上下坡、偏离赛道等多种状态。

/*车辆运行状态组结束*/

class StateMachine
{
private:
	State *current_state;
public:
	StateMachine();
	virtual ~StateMachine();
	State* RetrieveState();
	void SendCommand(uint8_t command);
	void SendInput();
	void Reset();
	template <class StateToSet> void ForceSetState();
};

#endif //#ifndef STATE_MACHINE_H
