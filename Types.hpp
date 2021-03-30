#pragma once

//#include <GL/gl.h>
//#include <GL/glext.h>
//#include <GL/glew.h>

#include <vector>
#include <string>
#include <algorithm>

namespace glgen {

struct LooseType {
	std::string group;
	std::string typeString;
};

struct ParamInfo {
	std::string name;
	LooseType type;
};

struct FunctionInfo {
	std::string name;
	std::vector<ParamInfo> paramInfos;
	LooseType returnType;

	std::string getPrettyName() const;
};

struct EnumInfo {
	std::string name;
	std::string value;
	std::string getPrettyName() const;
};

struct Version {
	std::vector<EnumInfo> enums;
	std::vector<FunctionInfo> functions;
	std::string api;
	int major;
	int minor;
};
}