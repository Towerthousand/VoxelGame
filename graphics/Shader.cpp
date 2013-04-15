#include "Shader.hpp"

Shader::Shader() {
}

Shader::~Shader() {
}

GLuint& Shader::getHandle() {
	return shaderHandle;
}

bool Shader::loadFromFile(GLenum type, const std::string& filePath) {
	std::vector<char> fileContents;
	int length;
	if (!getFileContents(filePath,fileContents,length)) {
		outLog("#ERROR Failed to get the contents from " + filePath);
		return false;
	}
	shaderHandle = glCreateShader(type);
	for (int i = 0; i < fileContents.size(); ++i)
		std::cout << fileContents[i];
	std::cout << std::endl;
	glShaderSource(shaderHandle, 1, (const GLchar**)&fileContents, &length);
	glCompileShader(shaderHandle);
	int compiled;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		outLog( "#ERROR Failed to compile: " + filePath);
		glDeleteShader(shaderHandle);
		return false;
	}
	else outLog( " - Compiled " + filePath + " successfully.");
	return true;
}

bool Shader::getFileContents(const std::string& filePath, std::vector<char>& buffer, int &fileLength) {
	std::ifstream file(filePath.c_str(), std::ios_base::binary);
	if (!file)
		return false;
	file.seekg(0, std::ios_base::end);
	fileLength = file.tellg();
	if (fileLength > 0) {
		file.seekg(0, std::ios_base::beg);
		buffer.resize(static_cast<std::size_t>(fileLength));
		file.read(&buffer[0], fileLength);
	}
	buffer.push_back('\0');
	return true;
}
