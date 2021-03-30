//
// Created by spirit on 17.01.2021.
//

#include <set>
#include "Formatter.hpp"
#include "Writer.hpp"

namespace glgen {

static std::vector<std::string> headerIncludes = {
		"<cstdint>"
};

static std::vector<std::string> sourceIncludes = {
};

static std::vector<std::pair<std::string, std::string>> typedefs = {
		{"GLenum",         "std::uint32_t"},
		{"GLboolean",      "std::uint8_t"},
		{"GLvoid",         "void"},
		{"GLbyte",         "std::int8_t"},
		{"GLubyte",        "std::uint8_t"},
		{"GLshort",        "std::int16_t"},
		{"GLushort",       "std::uint16_t"},
		{"GLint",          "std::int32_t"},
		{"GLuint",         "std::uint32_t"},
		{"GLclampx",       "std::int32_t"},
		{"GLsizei",        "std::int32_t"},
		{"GLfloat",        "float"},
		{"GLclampf",       "float"},
		{"GLdouble",       "double"},
		{"GLclampd",       "double"},
		{"GLchar",         "char"},
		{"GLhalf",         "std::uint16_t"},
		{"GLhalfARB",      "std::uint16_t"},
		{"GLfixed",        "std::int32_t"},
		{"GLintptr",       "std::intptr_t"},
		{"GLintptrARB",    "std::intptr_t"},
		{"GLsizeiptr",     "std::intptr_t"},
		{"GLsizeiptrARB",  "std::intptr_t"},
		{"GLint64",        "std::int64_t"},
		{"GLint64EXT",     "std::int64_t"},
		{"GLuint64",       "std::uint64_t"},
		{"GLuint64EXT",    "std::uint64_t"},
		{"GLbitfield",     "std::uint32_t"},
		{"GLDEBUGPROC",    "void (*)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam)"},
		{"GLsync",         "void*"},
		{"FunctionLoader", "void*(const char*)"}
};

static void writeDefFile(Writer &writer) {
	std::string text = "/**********************************************************************\n"
	                   " * Begin system-specific stuff.\n"
	                   " */\n"
	                   "\n"
	                   "#if defined(_WIN32) && !defined(__WIN32__) && !defined(__CYGWIN__)\n"
	                   "#define __WIN32__\n"
	                   "#endif\n"
	                   "\n"
	                   "#if defined(__WIN32__) && !defined(__CYGWIN__)\n"
	                   "#  if (defined(_MSC_VER) || defined(__MINGW32__)) && defined(BUILD_GL32) /* tag specify we're building mesa as a DLL */\n"
	                   "#    define GLAPI __declspec(dllexport)\n"
	                   "#  elif (defined(_MSC_VER) || defined(__MINGW32__)) && defined(_DLL) /* tag specifying we're building for DLL runtime support */\n"
	                   "#    define GLAPI __declspec(dllimport)\n"
	                   "#  else /* for use with static link lib build of Win32 edition only */\n"
	                   "#    define GLAPI extern\n"
	                   "#  endif\n"
	                   "#  if defined(__MINGW32__) && defined(GL_NO_STDCALL) || defined(UNDER_CE)  /* The generated DLLs by MingW with STDCALL are not compatible with the ones done by Microsoft's compilers */\n"
	                   "#    define GLAPIENTRY\n"
	                   "#  else\n"
	                   "#    define GLAPIENTRY __stdcall\n"
	                   "#  endif\n"
	                   "#elif defined(__CYGWIN__) && defined(USE_OPENGL32) /* use native windows opengl32 */\n"
	                   "#  define GLAPI extern\n"
	                   "#  define GLAPIENTRY __stdcall\n"
	                   "#elif (defined(__GNUC__) && __GNUC__ >= 4) || (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590))\n"
	                   "#  define GLAPI __attribute__((visibility(\"default\")))\n"
	                   "#  define GLAPIENTRY\n"
	                   "#endif /* WIN32 && !CYGWIN */\n"
	                   "\n"
	                   "/*\n"
	                   " * WINDOWS: Include windows.h here to define APIENTRY.\n"
	                   " * It is also useful when applications include this file by\n"
	                   " * including only glut.h, since glut.h depends on windows.h.\n"
	                   " * Applications needing to include windows.h with parms other\n"
	                   " * than \"WIN32_LEAN_AND_MEAN\" may include windows.h before\n"
	                   " * glut.h or gl.h.\n"
	                   " */\n"
	                   "#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__)\n"
	                   "#ifndef WIN32_LEAN_AND_MEAN\n"
	                   "#define WIN32_LEAN_AND_MEAN 1\n"
	                   "#endif\n"
	                   "#include <windows.h>\n"
	                   "#endif\n"
	                   "\n"
	                   "#ifndef GLAPI\n"
	                   "#define GLAPI extern\n"
	                   "#endif\n"
	                   "\n"
	                   "#ifndef GLAPIENTRY\n"
	                   "#define GLAPIENTRY\n"
	                   "#endif\n"
	                   "\n"
	                   "#ifndef APIENTRY\n"
	                   "#define APIENTRY GLAPIENTRY\n"
	                   "#endif\n"
	                   "\n"
	                   "/* \"P\" suffix to be used for a pointer to a function */\n"
	                   "#ifndef APIENTRYP\n"
	                   "#define APIENTRYP APIENTRY *\n"
	                   "#endif\n"
	                   "\n"
	                   "#ifndef GLAPIENTRYP\n"
	                   "#define GLAPIENTRYP GLAPIENTRY *\n"
	                   "#endif\n"
	                   "\n"
	                   "/*\n"
	                   " * End system-specific stuff.\n"
	                   " **********************************************************************/";
	writer.writeLine(text);
}

Formatter::Formatter() :
		className("GL"),
		classNamespace("OpenGL"),
		headerExtension(".hpp"),
		sourceExtension(".cpp") {
}

void Formatter::generate(const std::vector<Version> &versions, std::string outputPath) {
	if (outputPath.find_last_of('/') != outputPath.size() - 1) {
		outputPath += '/';
	}

	{
		Writer writer(outputPath + className + ".inl");
		writeDefFile(writer);
	}

	generateHeader(versions, outputPath);
	generateSource(versions, outputPath);
}

const std::string &Formatter::getClassNamespace() const {
	return classNamespace;
}

void Formatter::setClassNamespace(const std::string &name) {
	this->classNamespace = name;
}

const std::string &Formatter::getClassName() const {
	return className;
}

void Formatter::setClassName(const std::string &name) {
	this->className = name;
}

const std::string &Formatter::getHeaderExtension() const {
	return headerExtension;
}

void Formatter::setHeaderExtension(const std::string &extension) {
	headerExtension = extension;
}

const std::string &Formatter::getSourceExtension() const {
	return sourceExtension;
}

void Formatter::setSourceExtension(const std::string &extension) {
	sourceExtension = extension;
}

void Formatter::generateHeader(const std::vector<Version> &versions, const std::string &path) {
	Writer writer(path + className + headerExtension);

	writer.writeLine("#pragma once");
	writer.writeLine();
	for (const auto &include : headerIncludes) writer.write("#include \"").write(className).writeLine(".inl\"");
	for (const auto &include : headerIncludes) writer.write("#include ").writeLine(include);
	writer.writeLine();

	writer.write("namespace ").write(classNamespace).writeLine(" {");

	writer.writeLine();
	for (const auto &typeDef : typedefs)
		writer.write("using ")
		      .write(typeDef.first)
		      .write(" = ")
		      .write(typeDef.second)
		      .writeLine(";");
	writer.writeLine();

	writer.write("class ").write(className).writeLine(" {");
	writer.writeLine("public:");
	writer.indent();
	writer.writeLine();

	writer.write("static void SetCurrent(").write(className).writeLine("* gl);");
	writer.writeLine();

	writer.write(className).writeLine("(FunctionLoader loader);");
	writer.writeLine();

	std::set<std::string> usedEnums;
	std::set<std::string> usedFunctions;

	for (const auto &version : versions) {
		if (version.api != "gl") continue;
		writer.write("// version ")
		      .write(version.api)
		      .write(std::to_string(version.major))
		      .write(".")
		      .write(std::to_string(version.minor))
		      .writeLine();

		writer.writeLine("enum {");
		writer.indent();
		bool isFirst = true;
		for (const auto &enumInfo : version.enums) {
			if (usedEnums.find(enumInfo.name) != usedEnums.end()) continue;
			usedEnums.insert(enumInfo.name);
			if (!isFirst) {
				writer.writeLine(",");
			}

			writer.write(enumInfo.getPrettyName())
			      .write(" = ")
			      .write(enumInfo.value);
			isFirst = false;
		}
		writer.writeLine();
		writer.unIndent();
		writer.writeLine("};");

		writer.writeLine();

		for (const auto &funcInfo : version.functions) {
			if (usedFunctions.find(funcInfo.name) != usedFunctions.end()) continue;
			usedFunctions.insert(funcInfo.name);

			writer.write("static ")
			      .write(funcInfo.returnType.typeString)
			      .write(" ")
			      .write(funcInfo.getPrettyName())
			      .write("(");
			bool firstParam = true;
			for (const auto &param : funcInfo.paramInfos) {
				if (!firstParam) writer.write(", ");

				writer.write(param.type.typeString).write(" ").write(param.name);

				firstParam = false;
			}
			writer.writeLine(");");
		}

		writer.writeLine();

	}

	writer.writeLine();
	writer.unIndent();
	writer.writeLine("private:");
	writer.indent();

	usedFunctions.clear();
	for (const auto &version : versions) {
		if (version.api != "gl") continue;

		for (const auto &funcInfo : version.functions) {
			if (usedFunctions.find(funcInfo.name) != usedFunctions.end()) continue;
			usedFunctions.insert(funcInfo.name);

			writer.write(funcInfo.returnType.typeString)
			      .write(" (APIENTRYP _")
			      .write(funcInfo.name)
			      .write(")(");
			bool firstParam = true;
			for (const auto &param : funcInfo.paramInfos) {
				if (!firstParam) writer.write(", ");

				writer.write(param.type.typeString).write(" ").write(param.name);

				firstParam = false;
			}
			writer.writeLine(");");
		}

		writer.writeLine();

	}

	writer.unIndent();

	writer.writeLine();
	writer.write("}; //").writeLine(className);

	writer.write("} //").writeLine(classNamespace);
}

void Formatter::generateSource(const std::vector<Version> &versions, const std::string &path) {
	Writer writer(path + className + sourceExtension);

	writer.write("#include \"").write(className).write(headerExtension).writeLine("\"");
	writer.writeLine();
	if (!sourceIncludes.empty()) {
		for (const auto &include : sourceIncludes) writer.write("#include ").writeLine(include);
		writer.writeLine();
	}

	writer.write("namespace ").write(classNamespace).writeLine(" {")
	      .writeLine()
	      .writeLine("static thread_local ").write(className).writeLine("* _local_instance = nullptr;")
	      .writeLine()
	      .writeLine("void ").write(className).writeLine("::SetCurrent(GL *instance) {\n")
	      .indent()
	      .writeLine("_local_instance = instance;\n")
	      .unIndent()
	      .writeLine("}")
	      .writeLine("template<typename T>")
	      .writeLine("static void _load_function(FunctionLoader loader, T& ptr, const char* name) {")
	      .indent()
	      .writeLine("ptr = (T) loader(name);")
	      .unIndent()
	      .writeLine("}")
	      .writeLine()
	      .write(className).write("::").write(className).writeLine("(FunctionLoader loader) {")
	      .indent();

	{
		std::set<std::string> usedFunctions;
		for (const auto &version : versions) {
			if (version.api != "gl") continue;
			for (const auto &funcInfo : version.functions) {
				if (usedFunctions.find(funcInfo.name) != usedFunctions.end()) continue;
				usedFunctions.insert(funcInfo.name);
				writer.write("_load_function(loader, _")
				      .write(funcInfo.name)
				      .write(", \"")
				      .write(funcInfo.name)
				      .write("\");")
				      .writeLine();

			}
		}
	}
	writer.unIndent()
	      .writeLine("}")
	      .writeLine();

	{
		std::set<std::string> usedFunctions;
		for (const auto &version : versions) {
			if (version.api != "gl") continue;
			for (const auto &funcInfo : version.functions) {
				if (usedFunctions.find(funcInfo.name) != usedFunctions.end()) continue;
				usedFunctions.insert(funcInfo.name);

				writer.write(funcInfo.returnType.typeString)
				      .write(" ")
				      .write(className)
				      .write("::")
				      .write(funcInfo.getPrettyName())
				      .write("(");
				{
					bool firstParam = true;
					for (const auto &param : funcInfo.paramInfos) {
						if (!firstParam) writer.write(", ");
						writer.write(param.type.typeString).write(" ").write(param.name);
						firstParam = false;
					}
				}
				writer.writeLine(") {");
				writer.indent();

				if (funcInfo.returnType.typeString != "void") {
					writer.write("return ");
				}
				writer.write("_local_instance->_")
				      .write(funcInfo.name)
				      .write("(");
				{
					bool firstParam = true;
					for (const auto &param : funcInfo.paramInfos) {
						if (!firstParam) writer.write(", ");
						writer.write(param.name);
						firstParam = false;
					}
				}
				writer.writeLine(");")
				      .unIndent()
				      .writeLine("}")
				      .writeLine();

			}
		}
	}

	writer.writeLine();
	writer.writeLine("}");
}

}
