#include "ShaderManager.hpp"

ShaderManager::ShaderManager() {
}

ShaderManager::~ShaderManager() {
}

bool ShaderManager::loadVertexShader(const std::string &filePath, const std::string &shaderID) {
	outLog("* Loading new vertex shader: \"" + shaderID + "\" from " + filePath);
	Shader newShader;
	if (!newShader.loadFromFile(GL_VERTEX_SHADER,filePath)) {
		outLog("#ERROR Failed to load vertex shader resource as \"" + shaderID + "\" from filepath " + filePath);
		return false;
	}
	vertexShaders[shaderID] = newShader;
	return true;
}

bool ShaderManager::loadFragmentShader(const std::string &filePath, const std::string &shaderID) {
	outLog("* Loading new fragment shader: \"" + shaderID + "\" from " + filePath);
	Shader newShader;
	if (!newShader.loadFromFile(GL_FRAGMENT_SHADER,filePath)) {
		outLog("#ERROR Failed to load fragment shader resource as \"" + shaderID + "\" from filepath " + filePath);
		return false;
	}
	fragmentShaders[shaderID] = newShader;
	return true;
}

bool ShaderManager::makeProgram(const std::string& vertexShaderID, const std::string& fragmentShaderID, const std::string& programID) {
	outLog("* Creating new shader program: \"" + programID + "\"" +
		   "\" with vertex shader \"" + vertexShaderID +
		   "\" and fragment \"" + fragmentShaderID + "\"");
	int linked;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaders[vertexShaderID].getHandle());
	glAttachShader(program, fragmentShaders[fragmentShaderID].getHandle());
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		outLog("#ERROR Failed to link shader program \"" + programID +
			   "\" with vertex shader \"" + vertexShaderID +
			   "\" and fragment \"" + fragmentShaderID + "\"");
		glDeleteProgram(program);
		return false;
	}
	shaderPrograms[programID] = program;
	return true;
}

void ShaderManager::useProgram(const std::string &programID) {
	GLuint program = shaderPrograms[programID];
	GLint current;
	glGetIntegerv(GL_CURRENT_PROGRAM,&current);
	if(GLuint(current) != program)
		glUseProgram(program);
}

GLint ShaderManager::getUniLoc(const std::string& programID, const std::string &uniformID) {
	GLuint program = shaderPrograms[programID];
	useProgram(programID);
	GLint location = glGetUniformLocation(program, (GLchar *) uniformID.c_str());
	if (location == -1)
		outLog("#ERROR When trying to get uniform from \"" + programID + "\": no uniform named " + uniformID);
	return location;
}

/////////////////////////////////////////////FLOATS

void ShaderManager::sendUniform1f(const std::string& programID,const std::string& uniformID, float x) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform1f(location, x);
}

void ShaderManager::sendUniform2f(const std::string& programID,const std::string& uniformID
						   , float x, float y) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform2f(location, x, y);
}

void ShaderManager::sendUniform2f(const std::string& programID,const std::string& uniformID, const vec2f& v) {
	sendUniform2f(programID, uniformID, v.x, v.y);
}

void ShaderManager::sendUniform3f(const std::string& programID,const std::string& uniformID
						   , float x, float y, float z) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform3f(location, x, y, z);
}

void ShaderManager::sendUniform3f(const std::string& programID,const std::string& uniformID, const vec3f& v) {
	sendUniform3f(programID, uniformID, v.x, v.y, v.z);
}

void ShaderManager::sendUniform4f(const std::string& programID,const std::string& uniformID
						   , float x, float y, float z, float w) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform4f(location, x, y, z, w);
}

void ShaderManager::sendUniform4f(const std::string& programID,const std::string& uniformID, const vec4f& v){
	sendUniform4f(programID, uniformID, v.x, v.y, v.z, v.w);
}

/////////////////////////////////////////////INTEGERS

void ShaderManager::sendUniform1i(const std::string& programID,const std::string& uniformID, int x) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform1i(location, x);
}

void ShaderManager::sendUniform2i(const std::string& programID,const std::string& uniformID
						   , int x, int y) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform2i(location, x, y);
}

void ShaderManager::sendUniform2i(const std::string& programID,const std::string& uniformID, const vec2i& v) {
	sendUniform2i(programID, uniformID, v.x, v.y);
}

void ShaderManager::sendUniform3i(const std::string& programID,const std::string& uniformID
						   , int x, int y, int z) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform3i(location, x, y, z);
}

void ShaderManager::sendUniform3i(const std::string& programID,const std::string& uniformID, const vec3i& v) {
	sendUniform3i(programID, uniformID, v.x, v.y, v.z);
}

void ShaderManager::sendUniform4i(const std::string& programID,const std::string& uniformID
						   , int x, int y, int z, int w) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform4i(location, x, y, z, w);
}

void ShaderManager::sendUniform4i(const std::string& programID,const std::string& uniformID, const vec4i& v){
	sendUniform4i(programID, uniformID, v.x, v.y, v.z, v.w);
}

/////////////////////////////////////////////UNSIGNED INTEGERS

void ShaderManager::sendUniform1ui(const std::string& programID,const std::string& uniformID, uint x) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform1ui(location, x);
}

void ShaderManager::sendUniform2ui(const std::string& programID,const std::string& uniformID
							, uint x, uint y) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform2ui(location, x, y);
}

void ShaderManager::sendUniform2ui(const std::string& programID,const std::string& uniformID, const vec2ui& v) {
	sendUniform2ui(programID, uniformID, v.x, v.y);
}

void ShaderManager::sendUniform3ui(const std::string& programID,const std::string& uniformID
							, uint x, uint y, uint z) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform3ui(location, x, y, z);
}

void ShaderManager::sendUniform3ui(const std::string& programID,const std::string& uniformID, const vec3ui& v) {
	sendUniform3ui(programID, uniformID, v.x, v.y, v.z);
}

void ShaderManager::sendUniform4ui(const std::string& programID,const std::string& uniformID
							, uint x, uint y, uint z, uint w) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniform4ui(location, x, y, z, w);
}

void ShaderManager::sendUniform4ui(const std::string& programID,const std::string& uniformID, const vec4ui& v){
	sendUniform4ui(programID, uniformID, v.x, v.y, v.z, v.w);
}

/////////////////////////////////////////////MATRIX

void ShaderManager::sendUniformMat2f(const std::string& programID,const std::string& uniformID
							  , const float * value) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniformMatrix2fv(location, 1, GL_FALSE, value);
}

void ShaderManager::sendUniformMat3f(const std::string& programID,const std::string& uniformID
							  , const float * value) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniformMatrix3fv(location, 1, GL_FALSE, value);
}

void ShaderManager::sendUniformMat4f(const std::string& programID,const std::string& uniformID
							  , const float * value) {
	GLuint program = shaderPrograms[programID];
	glUseProgram(program);
	GLint location = getUniLoc(programID, uniformID);
	if (location == -1) //no such uniform error
		return;
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}
