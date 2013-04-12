#include "Shader.hpp"

Shader::Shader() {
}

Shader::~Shader() {
}

bool Shader::loadFromFile(GLenum type, std::string filePath) {
	std::ifstream file;
	file.open(filePath,std::ios::in);
	if(!file)
		return false; //Could not open file

	int length;
	std::vector<char> fileContents;
	if (!getFileContents("resources/vertex.glsl",fileContents,length))
		return false;
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&fileContents, &length);
	glCompileShader(shader);
	int shader_ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	  if (!shader_ok) {
		  outLog( "#ERROR Failed to compile: " + filePath);
		  glDeleteShader(shader);
		  return false;
	  }
	  else outLog(toString(shader_ok));
	return true;
}

bool Shader::getFileContents(const std::string& filename, std::vector<char>& buffer, int &fileLength) {
	std::ifstream file(filename.c_str(), std::ios_base::binary);
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
