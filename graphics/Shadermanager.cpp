#include "Shadermanager.hpp"

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
	if (!newShader.loadFromFile(GL_VERTEX_SHADER,filePath)) {
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
	return true;
}

GLint ShaderManager::getUniLoc(const std::string& programID, const std::string &uniformID) {
	GLuint program = shaderPrograms[programID];
	glUseProgram();
	GLint loc = glGetUniformLocation(programID, name);

	if (loc == -1)
		printf("No such uniform named \"%s\"\n", name);

	checkGLError("Get uni loc");  // Check for OpenGL errors
	return loc;
}

void Shader::setParameter(const std::string& shaderID,const std::string& name, float x) {
	GLuint program = shaderPrograms[shaderID];

	// Enable program
	glUseProgram(program);

	// Get parameter location and assign it new values
	GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
	if (location != -1)
		glCheck(glUniform1fARB(location, x));
	else
		err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

	// Disable program
	glCheck(glUseProgramObjectARB(program));
}

void Shader::setParameter(const std::string& name, float x, float y)
{
	if (m_shaderProgram)
	{
		ensureGlContext();

		// Enable program
		GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
		glCheck(glUseProgramObjectARB(m_shaderProgram));

		// Get parameter location and assign it new values
		GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
		if (location != -1)
			glCheck(glUniform2fARB(location, x, y));
		else
			err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

		// Disable program
		glCheck(glUseProgramObjectARB(program));
	}
}

void Shader::setParameter(const std::string& name, float x, float y, float z)
{
	if (m_shaderProgram)
	{
		ensureGlContext();

		// Enable program
		GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
		glCheck(glUseProgramObjectARB(m_shaderProgram));

		// Get parameter location and assign it new values
		GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
		if (location != -1)
			glCheck(glUniform3fARB(location, x, y, z));
		else
			err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

		// Disable program
		glCheck(glUseProgramObjectARB(program));
	}
}

void Shader::setParameter(const std::string& name, float x, float y, float z, float w)
{
	if (m_shaderProgram)
	{
		ensureGlContext();

		// Enable program
		GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
		glCheck(glUseProgramObjectARB(m_shaderProgram));

		// Get parameter location and assign it new values
		GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
		if (location != -1)
			glCheck(glUniform4fARB(location, x, y, z, w));
		else
			err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

		// Disable program
		glCheck(glUseProgramObjectARB(program));
	}
}

void Shader::setParameter(const std::string& name, const Vector2f& v)
{
	setParameter(name, v.x, v.y);
}

void Shader::setParameter(const std::string& name, const Vector3f& v)
{
	setParameter(name, v.x, v.y, v.z);
}

void Shader::setParameter(const std::string& name, const Color& color)
{
	setParameter(name, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}

void Shader::setParameter(const std::string& name, const sf::Transform& transform)
{
	if (m_shaderProgram)
	{
		ensureGlContext();

		// Enable program
		GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
		glCheck(glUseProgramObjectARB(m_shaderProgram));

		// Get parameter location and assign it new values
		GLint location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
		if (location != -1)
			glCheck(glUniformMatrix4fvARB(location, 1, GL_FALSE, transform.getMatrix()));
		else
			err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

		// Disable program
		glCheck(glUseProgramObjectARB(program));
	}
}

void Shader::setParameter(const std::string& name, const Texture& texture)
{
	if (m_shaderProgram)
	{
		ensureGlContext();

		// Find the location of the variable in the shader
		int location = glGetUniformLocationARB(m_shaderProgram, name.c_str());
		if (location == -1)
		{
			err() << "Texture \"" << name << "\" not found in shader" << std::endl;
			return;
		}

		// Store the location -> texture mapping
		TextureTable::iterator it = m_textures.find(location);
		if (it == m_textures.end())
		{
			// New entry, make sure there are enough texture units
			static const GLint maxUnits = getMaxTextureUnits();
			if (m_textures.size() + 1 >= static_cast<std::size_t>(maxUnits))
			{
				err() << "Impossible to use texture \"" << name << "\" for shader: all available texture units are used" << std::endl;
				return;
			}

			m_textures[location] = &texture;
		}
		else
		{
			// Location already used, just replace the texture
			it->second = &texture;
		}
	}
}

void Shader::setParameter(const std::string& name, CurrentTextureType)
{
	if (m_shaderProgram)
	{
		ensureGlContext();

		// Find the location of the variable in the shader
		m_currentTexture = glGetUniformLocationARB(m_shaderProgram, name.c_str());
		if (m_currentTexture == -1)
			err() << "Texture \"" << name << "\" not found in shader" << std::endl;
	}
}

void Shader::bind(const Shader* shader)
{
	ensureGlContext();

	if (shader && shader->m_shaderProgram)
	{
		// Enable the program
		glCheck(glUseProgramObjectARB(shader->m_shaderProgram));

		// Bind the textures
		shader->bindTextures();

		// Bind the current texture
		if (shader->m_currentTexture != -1)
			glCheck(glUniform1iARB(shader->m_currentTexture, 0));
	}
	else
	{
		// Bind no shader
		glCheck(glUseProgramObjectARB(0));
	}
}
