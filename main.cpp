#include <iostream>
#include "Types.hpp"
#include "Parser.hpp"
#include "Formatter.hpp"

int main() {

	glgen::EnumInfo d;

	d.name = "GL_VERSION_NAME_STRING";
	std::cout << "==================\n";

	auto versions = glgen::Parser::parse("gl.xml");

	glgen::Formatter formatter;
    formatter.setClassNamespace("OpenGL");
	formatter.generate(versions, "./glout");


	std::cout << "==================\n";

	return 0;
}
