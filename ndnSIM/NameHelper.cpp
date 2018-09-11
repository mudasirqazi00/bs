#ifndef NAME_HELPER_CPP
#define NAME_HELPER_CPP

#include <iostream>
#include <string>
#include <vector>
#include "../src/ndnSIM/ndn-cxx/src/name.hpp"

using namespace std;

class NameHelper {
public:

	static std::string join(std::vector<ndn::Name> v) {
		std::string names = "";
		for (unsigned int k = 0; k < v.size(); k++)
			names += v[k].toUri() + "+";

		if (names.size() > 0)
			names = names.substr(0, names.size() - 1);

		return names;
	}

	static std::vector<ndn::Name> split(std::string str) {
		std::vector<ndn::Name> v;
		std::string delimiter = "+";
		size_t pos = 0;
		std::string token = "";
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			if (!token.empty() && token != delimiter)
				v.push_back(token);

			str.erase(0, pos + delimiter.length());
		}

		if (!str.empty() && str != "+")
			v.push_back(str);

		return v;
	}
};
#endif
