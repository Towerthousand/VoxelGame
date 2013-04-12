#ifndef SHADER_HPP
#define SHADER_HPP
#include "tools.hpp"

class Shader {
	public:
		Shader();
		~Shader();

		bool loadFromFile(GLenum type, std::string filePath);
		GLuint &getHandle();

	private:
		bool getFileContents(const std::string& filename, std::vector<char>& buffer, int &fileLength);
		GLuint shader;

};
#endif // SHADER_HPP
