#include "Parser.hpp"

namespace glgen {

std::vector<Version> Parser::parse(const std::string& filename) {
	tinyxml2::XMLDocument xmlDocument;
	xmlDocument.LoadFile(filename.c_str());
	auto registry = xmlDocument.FirstChildElement("registry");

	std::vector<FunctionInfo> functionInfos = parseFunctions(registry);
	std::vector<EnumInfo> enumInfos = parseEnums(registry);
	std::vector<Version> versionInfos;

	for (auto node = registry->FirstChildElement("feature"); node; node = node->NextSiblingElement("feature"))
	{
		std::string versionStr = node->Attribute("number");
		Version version;
		version.api = node->Attribute("api");
		version.major = std::stoi(versionStr.substr(0, versionStr.find('.')));
		version.minor = std::stoi(versionStr.substr(versionStr.find('.') + 1));

		for (auto require = node->FirstChildElement("require"); require; require = require->NextSiblingElement("require"))
		{
			for (auto requireElement = require->FirstChildElement(); requireElement; requireElement = requireElement->NextSiblingElement())
			{
				if (std::strcmp(requireElement->Name() , "enum") == 0)
				{
					std::string enumName = requireElement->Attribute("name");
					for(const auto& enumInfo : enumInfos) {
						if(enumInfo.name == enumName) {
							version.enums.push_back(enumInfo);
							break;
						}
					}
				}
				else if (std::strcmp(requireElement->Name() , "command") == 0)
				{
					std::string funcName = requireElement->Attribute("name");
					for(const auto& funcInfo : functionInfos) {
						if(funcInfo.name == funcName) {
							version.functions.push_back(funcInfo);
							break;
						}
					}
				}
			}
		}
		versionInfos.push_back(version);
	}

	return versionInfos;
}

std::vector<FunctionInfo> Parser::parseFunctions(tinyxml2::XMLElement *registry) {
	std::vector<FunctionInfo> functionInfos;

	auto commands = registry->FirstChildElement("commands");

	for (auto element = commands->FirstChildElement("command"); element; element = element->NextSiblingElement(
			"command")) {
		//<proto>void <name>glAlphaFragmentOp1ATI</name></proto>
		functionInfos.push_back(parseCommandNode(element));
	}

	return functionInfos;
}

std::vector<EnumInfo> Parser::parseEnums(tinyxml2::XMLElement *registry) {
	std::vector<EnumInfo> enumInfos;

	for (auto element = registry->FirstChildElement("enums"); element ; element = element->NextSiblingElement("enums"))
	{
		for (auto enumNode = element->FirstChildElement("enum"); enumNode; enumNode = enumNode->NextSiblingElement("enum"))
		{
			EnumInfo enumInfo;
			enumInfo.name = enumNode->Attribute("name");
			enumInfo.value = enumNode->Attribute("value");
			enumInfos.push_back(enumInfo);
		}
	}
	return enumInfos;
}

FunctionInfo Parser::parseCommandNode(tinyxml2::XMLElement *element) {
	FunctionInfo functionInfo;
	{
		if(auto protoElem = element->FirstChildElement("proto")) {
			functionInfo.name = extractName(protoElem);
			functionInfo.returnType = extractType(protoElem);
		}

		for (auto child = element->FirstChildElement("param"); child; child = child->NextSiblingElement("param")) {
			functionInfo.paramInfos.push_back(parseParam(child));
		}
	}

	return functionInfo;
}

LooseType Parser::extractType(tinyxml2::XMLElement *element) {

	LooseType looseType;

	std::string retType;

	for (auto childNode = element->FirstChild(); childNode; childNode = childNode->NextSibling()) {
		if (auto textElement = childNode->ToText()) {
			std::string text = textElement->Value();
			retType += text;
		} else if (auto elementNode = childNode->ToElement()) {
			if (strcmp("ptype", elementNode->Name()) == 0)
				retType += elementNode->FirstChild()->Value();
		}
	}

	while (retType.find(' ') == 0) {
		retType.erase(retType.begin());
	}

	while (retType.find_last_of(' ') == retType.size() - 1) {
		retType.erase(retType.end() - 1);
	}


	looseType.typeString = retType;
	looseType.group = element->Attribute("group") ? element->Attribute("group") : "";

	return looseType;
}

std::string Parser::extractName(tinyxml2::XMLElement *element) {
	return element->FirstChildElement("name")->FirstChild()->Value();
}

ParamInfo Parser::parseParam(tinyxml2::XMLElement *element) {
	ParamInfo res;
	res.name = extractName(element);
	res.type = extractType(element);
	return res;
}

}
