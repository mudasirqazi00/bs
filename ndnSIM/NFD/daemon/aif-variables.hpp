
#ifndef aif_variables_hpp
#define aif_variables_hpp

class Variables {

public:
	static int m_dynamicAccumulationSize;
	static void increaseAccumulation();
	static void decreaseAccumulation();
	static int getDynamicAccumulation();
};

#endif
