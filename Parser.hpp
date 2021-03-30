#pragma once

#include "Types.hpp"
#include "tinyxml2.h"

namespace glgen {

class Parser {
public:
	static std::vector<Version> parse(const std::string& filename);

private:

	static std::vector<FunctionInfo> parseFunctions(tinyxml2::XMLElement *registry);

	static std::vector<EnumInfo> parseEnums(tinyxml2::XMLElement *registry);

	static FunctionInfo parseCommandNode(tinyxml2::XMLElement *pElement);

	static LooseType extractType(tinyxml2::XMLElement *pElement);

	static std::string extractName(tinyxml2::XMLElement *pElement);

	static ParamInfo parseParam(tinyxml2::XMLElement *element);


};

}
