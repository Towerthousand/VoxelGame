#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP
#include "Shader.hpp"

class ShaderManager {
	public:
		ShaderManager();
		~ShaderManager();

		bool loadVertexShader(const std::string& filePath, const std::string& shaderID);
		bool loadFragmentShader(const std::string& filePath, const std::string& shaderID);
		bool makeProgram(const std::string &vertexShaderID, const std::string &fragmentShaderID, const std::string &programID);

	private:
		std::map<std::string,Shader> vertexShaders;
		std::map<std::string,Shader> fragmentShaders;
		std::map<std::string,GLuint> shaderPrograms;

};

#endif // SHADERMANAGER_HPP
