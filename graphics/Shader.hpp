#ifndef SHADER_HPP
#define SHADER_HPP
#include "tools.hpp"

class Shader {
	public:
		Shader();
		~Shader();

		bool loadFromFile(GLenum type, const std::string &filePath);
		bool getFileContents(const std::string& filePath, std::vector<char>& buffer, int &fileLength);
		GLuint &getHandle();

	private:
		GLuint shaderHandle;
};
#endif // SHADER_HPP
