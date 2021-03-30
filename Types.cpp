#include "Types.hpp"

namespace glgen {

std::string EnumInfo::getPrettyName() const {
	std::string res = name;

	for(int i = 1; i < res.size(); i++) {

		if(res[i - 1] != '_') {
			res[i] = std::tolower(res[i]);
		}
	}

	while(res.find('_') != std::string::npos) {
		res.erase(res.begin() + res.find('_'));
	}

	return res;
}

std::string FunctionInfo::getPrettyName() const {
	return name.substr(2);
}

}

