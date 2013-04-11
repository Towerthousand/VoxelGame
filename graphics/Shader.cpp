#include "Shader.hpp"

Shader::Shader() {
}

Shader::~Shader() {
}

void Shader::loadFromFile(GLenum type, const char *filename) {
	GLint length;
	GLchar *source = file_contents(filename, &length);
	GLuint shader;
	GLint shader_ok;

	if (!source)
		return 0;
}
