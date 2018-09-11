
#ifndef StringManager_CPP
#define StringManager_CPP

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class StringManager {
public:
	static bool isWhitespace(std::string s){
	    for(unsigned int index = 0; index < s.length(); index++)
	        if(!std::isspace(s[index]))
	            return false;

	    return true;
	}

	static bool isValid(std::string s){
		if(s.empty())
			return false;

		if(s.size() > 0 && !s.empty() && !isWhitespace(s)  && s != "/" && s != "+")
			return true;
		else
			return false;
	}

	static std::vector<std::string> split(std::string str, std::string delimiter = "+"){
		std::vector<std::string> v;
		if(str.size() <= 0)
			return v;

		size_t pos = 0;
		std::string token = "";
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			if(isValid(token))
				v.push_back(token);

			str.erase(0, pos + delimiter.length());
		}

		if(isValid(str))
			v.push_back(str);

		return v;
	}

	static int splitSize(std::string str, std::string delimiter = "+"){
		return split(str, delimiter).size();
	}

	static std::string replace(std::string str, std::string replaceThis, std::string withThis) {
	    size_t start_pos = str.find(replaceThis);
	    if(start_pos == std::string::npos)
	        return str;

	    str.replace(start_pos, replaceThis.length(), withThis);
	    return str;
	}

	static std::string replaceAll(std::string str, std::string replaceThis, std::string withThis) {
	    if(replaceThis.empty())
	        return str;

	    size_t start_pos = 0;
	    while((start_pos = str.find(replaceThis, start_pos)) != std::string::npos) {
	        str.replace(start_pos, replaceThis.length(), withThis);
	        start_pos += withThis.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	    }
	    return str;
	}

	static std::string join(std::vector<std::string> v, std::string delimiter = "+"){
		std::string result = "";
		for(unsigned int k=0; k<v.size(); k++)
			if(isValid(v[k]))
				result += v[k] + delimiter;

		return result;
	}

	static std::string append(std::string str, std::string additional, std::string delimiter = "+"){
		if(additional.size() <= 0)
			return str;

		std::string result = str;
		result = result + delimiter + additional;
		return result;
	}

	static std::string remove(std::string mainStr, std::string toErase){
		if(toErase.size() <= 0)
			return mainStr;

		size_t pos = mainStr.find(toErase);
		if (pos != std::string::npos)
			mainStr.erase(pos, toErase.length());

		return mainStr;
	}

	static std::string removeAll(std::string mainStr, std::string toErase){
		if(toErase.size() <= 0)
			return mainStr;

		size_t pos = std::string::npos;
		while ((pos  = mainStr.find(toErase) )!= std::string::npos)
			mainStr.erase(pos, toErase.length());

		return mainStr;
	}

	static int find(std::string str, std::string sub){
		size_t pos = str.find(sub);
		if (pos != std::string::npos)
			return pos;
		else
			return -1;
	}

	static std::string getCleanString(std::string str, std::string delimiter = "+"){
		if(str.size() <= 0)
			return "";

		std::vector<std::string> result;
		std::vector<std::string> v = split(str, delimiter);
		for(unsigned int k=0; k<v.size(); k++)
			if(isValid(v[k]))
				result.push_back(v[k]);

		return join(result, delimiter);
	}

	static std::string getCleanString(std::vector<std::string> v, std::string delimiter = "+"){
		if(v.size() == 0)
			return "";

		std::vector<std::string> result;
		for(unsigned int k=0; k<v.size(); k++)
			if(isValid(v[k]))
				result.push_back(v[k]);

		return join(result, delimiter);
	}

	static std::vector<std::string> getCleanVector(std::string str, std::string delimiter = "+"){
		std::vector<std::string> result;
		if(str.size() <= 0)
			return result;

		std::vector<std::string> v = split(str, delimiter);
		for(unsigned int k=0; k<v.size(); k++)
			if(isValid(v[k]))
				result.push_back(v[k]);

		return result;
	}

	static std::vector<std::string> getCleanVector(std::vector<std::string> v, std::string delimiter = "+"){
		std::vector<std::string> result;
		if(v.size() <= 0)
			return result;

		for(unsigned int k=0; k<v.size(); k++)
			if(isValid(v[k]))
				result.push_back(v[k]);

		return result;
	}

	static std::string reset(std::string str){
		str = "";
		return str;
	}
};
#endif
