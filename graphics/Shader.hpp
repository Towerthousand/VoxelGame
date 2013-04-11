#ifndef SHADER_HPP
#define SHADER_HPP
#include "tools.hpp"

class Shader
{
	public:
		Shader();
		~Shader();

		bool loadFromFile(std::string filePath);
		GLuint &getHandle();

	private:
		GLuint shader;

#endif // SHADER_HPP
