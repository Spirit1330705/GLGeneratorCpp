//
// Created by spirit on 17.01.2021.
//

#pragma once

#include <vector>
#include "Types.hpp"

namespace glgen {

class Formatter {
public:

	Formatter();

	void generate(const std::vector<Version> &versions, std::string outputPath);

	const std::string &getClassNamespace() const;

	void setClassNamespace(const std::string &name);

	const std::string &getClassName() const;

	void setClassName(const std::string &name);

	const std::string &getHeaderExtension() const;

	void setHeaderExtension(const std::string &extension);

	const std::string &getSourceExtension() const;

	void setSourceExtension(const std::string &extension);

private:

	void generateHeader(const std::vector<Version> &versions, const std::string &path);

	void generateSource(const std::vector<Version> &versions, const std::string &path);

	std::string classNamespace;
	std::string className;
	std::string headerExtension;
	std::string sourceExtension;
};

}
