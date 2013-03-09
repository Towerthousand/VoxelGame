#ifndef Texture_HPP
#define Texture_HPP
#include "tools.hpp"

class Texture
{
	public:
		Texture();
		~Texture();

		bool load(const std::string& filePath);
		void bind();
		void setFilter(GLenum filter);
		void setWrap(GLenum wrap);

		GLuint getHandle();

		int getWidth();
		int getHeight();
	private:
		GLuint handle;
		sf::Vector2i size;
};

#endif // Texture_HPP
