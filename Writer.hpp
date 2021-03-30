//
// Created by spirit on 17.01.2021.
//

#pragma once

#include <string>
#include <fstream>

namespace glgen {

class Writer {
public:

	explicit Writer(const std::string &filename, const std::string& identString = "\t");

	Writer& write(const std::string& text);

	Writer& writeLine(const std::string& text = "");

	Writer& indent();

	Writer& unIndent();

	~Writer();

private:
	std::fstream fileStream;
	int indentCount;
	std::string outString;
	std::string identString;
};

}


