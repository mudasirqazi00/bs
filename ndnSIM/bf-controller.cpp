#ifndef bf_controller_cpp
#define bf_controller_cpp

#include <iostream>
#include <string>
#include "bf-timer.cpp"
using namespace std;

class BFController {

public:
	vector<string> m_deferredInterests;
	vector<string> m_deferredData;
	bool m_shouldFloodInterest = true;
	bool m_shouldFloodData = true;
	bool t_shouldFloodInterest = true;
	bool t_shouldFloodData = true;
	BFTimer timerInterest;
	BFTimer timerData;

	void startInterestTimer(){
		timerInterest.StartTimer(1); // 1 for interest timer
	}

	void startDataTimer(){
		timerData.StartTimer(2); // 2 for data timer
	}

	void checkInterest(string interests) {

//		LogManager::AddLogWithNodeId("bf-controller.cpp->checkInterest.interests", interests);

		if (interests.size() == 0){
			m_shouldFloodInterest = false;
			return;
		}

		while(!timerInterest.IsWaitOver()){
			//LogManager::AddLogWithNodeId("bf-controller.cpp->checkInterest.waiting...");
			if(existsInterest(interests))
				m_shouldFloodInterest = false;
			else
				m_shouldFloodInterest = true;

			addInterest(interests);
			timerInterest.UpdateTimer();
		}
	}

	void checkData(string data) {

//		LogManager::AddLogWithNodeId("bf-controller.cpp->checkData.data", data);

		if (data.size() == 0){
			m_shouldFloodData = false;
			return;
		}

		while(!timerData.IsWaitOver()){
//			LogManager::AddLogWithNodeId("bf-controller.cpp->checkData.waiting...");
			if(existsData(data))
				m_shouldFloodData = false;
			else
				m_shouldFloodData = true;

			addData(data);
			timerData.UpdateTimer();
		}
	}

	void clearInterests() {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.start");
		m_deferredInterests.clear();
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.completed");
	}

	void clearData() {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.start");
		m_deferredData.clear();
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.completed");
	}

	void addInterest(string interests) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->add.start.interests", interests);
		if (std::find(m_deferredInterests.begin(), m_deferredInterests.end(), interests) == m_deferredInterests.end()){
			//LogManager::AddLogWithNodeId("bf-controller.cpp->add.if");
			m_deferredInterests.push_back(interests);
		}
//		else
//			LogManager::AddLogWithNodeId("bf-controller.cpp->add.else");
	}

	void addData(string data) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->add.start.interests", interests);
		if (std::find(m_deferredData.begin(), m_deferredData.end(), data) == m_deferredData.end()){
			//LogManager::AddLogWithNodeId("bf-controller.cpp->add.if");
			m_deferredData.push_back(data);
		}
//		else
//			LogManager::AddLogWithNodeId("bf-controller.cpp->add.else");
	}

	bool existsInterest(string interests) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.start");
		for (unsigned int j = 0; j < (unsigned) m_deferredInterests.size(); j++){
			if (m_deferredInterests[j].compare(interests) == 0){
				//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.true");
				return true;
			}
		}

		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.false");
		return false;
	}

	bool existsData(string data) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.start");
		for (unsigned int j = 0; j < (unsigned) m_deferredData.size(); j++){
			if (m_deferredData[j].compare(data) == 0){
				//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.true");
				return true;
			}
		}

		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.false");
		return false;
	}

	bool shouldFloodInterest(){
		//LogManager::AddLogWithNodeId("bf-controller.cpp->shouldFloodInterest", m_shouldFloodInterest);
		return t_shouldFloodInterest;
	}

	bool shouldFloodData(){
		//LogManager::AddLogWithNodeId("bf-controller.cpp->shouldFloodInterest", m_shouldFloodInterest);
		return t_shouldFloodData;
	}
};
#endif
