
#include "aif-variables.hpp"
#include "aif-constants.cpp"
#include <iostream>
using namespace std;

int Variables::m_dynamicAccumulationSize = Constants::MinAccumulationSize;

void Variables::increaseAccumulation(){
	if(m_dynamicAccumulationSize != Constants::MaxAccumulationSize)
		m_dynamicAccumulationSize++;

	if(m_dynamicAccumulationSize == 2){
		m_dynamicAccumulationSize++;
	}
}

void Variables::decreaseAccumulation(){
	if(m_dynamicAccumulationSize != Constants::MinAccumulationSize)
		m_dynamicAccumulationSize--;

	if(m_dynamicAccumulationSize == 2){
		m_dynamicAccumulationSize--;
	}
}
int Variables::getDynamicAccumulation(){
	try{
		if(Constants::EnableDynamicAccumulation == 0)
			return Constants::StaticAccumulationSize;

		if(Constants::EnableDynamicAccumulation == 1){
			if(m_dynamicAccumulationSize < Constants::MinAccumulationSize || m_dynamicAccumulationSize > Constants::MaxAccumulationSize){
				m_dynamicAccumulationSize = Constants::MinAccumulationSize;
				return Constants::MinAccumulationSize;
			}

			return m_dynamicAccumulationSize;
		}
	} catch(std::exception& e){
		return Constants::MinAccumulationSize;
	}
}

