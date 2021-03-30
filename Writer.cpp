#include "Writer.hpp"

namespace glgen {

Writer::Writer(const std::string &filename, const std::string& identString) : indentCount(0), identString(identString) {
	fileStream.open(filename, std::ios_base::out);
}

Writer &Writer::write(const std::string &text) {
	outString += text;
	return *this;
}

Writer &Writer::writeLine(const std::string &text) {
	outString += text;
	for(int i = 0; i < indentCount; i++) fileStream << identString;
	fileStream << outString << '\n';
	outString = "";
	return *this;
}

Writer &Writer::indent() {
	indentCount++;
	return *this;
}

Writer &Writer::unIndent() {
	indentCount--;
	return * this;
}

Writer::~Writer() {
	fileStream.close();
}

}
