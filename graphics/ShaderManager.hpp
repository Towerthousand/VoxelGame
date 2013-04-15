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
		void useProgram(const std::string &programID);

		//FLOAT
		void sendUniform1f(const std::string& programID,const std::string& uniformID
					  , float x);
		void sendUniform2f(const std::string& programID,const std::string& uniformID
					  , const vec2f& v);
		void sendUniform2f(const std::string& programID,const std::string& uniformID
					  , float x, float y);
		void sendUniform3f(const std::string& programID,const std::string& uniformID
					  , const vec3f& v);
		void sendUniform3f(const std::string& programID,const std::string& uniformID
					  , float x, float y, float z);
		void sendUniform4f(const std::string& programID,const std::string& uniformID
					  , const vec4f& v);
		void sendUniform4f(const std::string& programID,const std::string& uniformID
					  , float x, float y, float z, float w);

		//INTEGER
		void sendUniform1i(const std::string& programID,const std::string& uniformID
					  , int x);
		void sendUniform2i(const std::string& programID,const std::string& uniformID
					  , const vec2i& v);
		void sendUniform2i(const std::string& programID,const std::string& uniformID
					  , int x, int y);
		void sendUniform3i(const std::string& programID,const std::string& uniformID
					  , const vec3i& v);
		void sendUniform3i(const std::string& programID,const std::string& uniformID
					  , int x, int y, int z);
		void sendUniform4i(const std::string& programID,const std::string& uniformID
					  , const vec4i& v);
		void sendUniform4i(const std::string& programID,const std::string& uniformID
					  , int x, int y, int z, int w);

		//UNSIGNED INTEGER
		void sendUniform1ui(const std::string& programID,const std::string& uniformID
					   , uint x);
		void sendUniform2ui(const std::string& programID,const std::string& uniformID
					  , const vec2ui& v);
		void sendUniform2ui(const std::string& programID,const std::string& uniformID
					   , uint x, uint y);
		void sendUniform3ui(const std::string& programID,const std::string& uniformID
					  , const vec3ui& v);
		void sendUniform3ui(const std::string& programID,const std::string& uniformID
					   , uint x, uint y, uint z);
		void sendUniform4ui(const std::string& programID,const std::string& uniformID
					  , const vec4ui& v);
		void sendUniform4ui(const std::string& programID,const std::string& uniformID
					   , uint x, uint y, uint z, uint w);

		//MATRIX
		void sendUniformMat2f(const std::string& programID,const std::string& uniformID
							  , const float * value);
		void sendUniformMat3f(const std::string& programID,const std::string& uniformID
							  , const float * value);
		void sendUniformMat4f(const std::string& programID,const std::string& uniformID
							  , const float * value);
	private:
		GLint getUniLoc(const std::string& programID, const std::string &uniformID);

		std::map<std::string,Shader> vertexShaders;
		std::map<std::string,Shader> fragmentShaders;
		std::map<std::string,GLuint> shaderPrograms;
};

#endif // SHADERMANAGER_HPP
