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
