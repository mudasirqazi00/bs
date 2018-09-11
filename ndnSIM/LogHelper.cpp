
#ifndef LOG_HELPER_CPP
#define LOG_HELPER_CPP

#include <iostream>
#include "../src/core/model/simulator.h"
#include "ns3/node.h"
#include "../src/network/model/node-list.h"
using namespace std;

class LogHelper {
public:
//	static void AddLog(std::string message, int number) {
//			std::string fileName = "./CppLogs.txt";
//			fstream myfile;
//			myfile.open(fileName, fstream::app);
//			if (myfile.is_open()) {
//				myfile << message << ":" << number << endl;
//				myfile.close();
//			}
//		}
	static const int GetNodeId() {
		uint32_t contextId = ns3::Simulator::GetContext();
		uint32_t nNodes = ns3::NodeList::GetNNodes();
		if(contextId < nNodes){
			ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(contextId);
			const int nodeId = node->GetId();
			return nodeId;
		}
		else
			return -1;
	}
};
#endif
