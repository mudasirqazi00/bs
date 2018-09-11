
#ifndef bf_timer_cpp
#define bf_timer_cpp

#include <ctime>
#include <iostream>
#include "LogManager.cpp"
#include "NFD/daemon/aif-constants.cpp"

using namespace std;

class BFTimer {
private:
	clock_t m_waitForSeconds = 0;
	bool m_waitOver = false;
	clock_t m_startTime = clock();
	clock_t m_endTime = clock();

public:
	void StartTimer(int timerFor){
		m_waitOver = false;

		if(timerFor == 1)
			m_waitForSeconds = GetIntervalForInterest();
		else if(timerFor == 2)
			m_waitForSeconds = GetIntervalForData();

		m_startTime = clock();
		m_endTime = (m_waitForSeconds * 1000) + m_startTime;
		if(clock() <= m_endTime)
			m_waitOver = false;
		else
			m_waitOver = true;
	}

	bool IsWaitOver(){
		//return m_waitOver;
		return true;
	}

	void UpdateTimer(){
		if(clock() <= m_endTime)
			m_waitOver = false;
		else
			m_waitOver = true;
	}

//	static int GetRandomInterval(){
//		int temp = HelperConstants::DEFERRED_DELAY_TIME_MAX;
//		srand (time(NULL));
//		temp = rand() % HelperConstants::DEFERRED_DELAY_TIME_MAX + HelperConstants::DEFERRED_DELAY_TIME_MIN;
//		LogManager::AddLogWithNodeId("HelperTimer.cpp->RandomTime", temp);
//		return temp;
//	}

	static int GetIntervalForInterest(){
		int temp = 0;
		srand (time(NULL));
		temp = (Constants::DeferredTime + rand() % Constants::DeferredTime) * Constants::DeferredTimeSlot;
		LogManager::AddLogWithNodeId("bf-timer.cpp->RandomIntervalForInterest", temp);
		return temp;
	}

	static int GetIntervalForData(){
		int temp = 0;
		srand (time(NULL));
		temp = (rand() % (Constants::DeferredTime - 1)) * Constants::DeferredTimeSlot;
		LogManager::AddLogWithNodeId("bf-timer.cpp->RandomIntervalForData", temp);
		return temp;
	}

	int GetRemainingTime(){
		return (int)((clock() - m_endTime) / 1000);
	}
};
#endif
