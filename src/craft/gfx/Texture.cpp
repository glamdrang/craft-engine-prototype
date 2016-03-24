#include "craft/common.h"
#include "Texture.h"

void Texture::load()
{
	image = IMG_Load(filename.c_str());
	if (!image) {
		std::cerr << "Failed to open image \"" << filename << "\" for a texture." << std::endl;
		return;
	}
}

void Texture::gl_load()
{
	glGenTextures(1, &this->tex_id);

	glBindTexture(GL_TEXTURE_2D, tex_id);

	int mode = GL_RGB;

	if (image->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::gl_unload()
{
	glDeleteTextures(1, &this->tex_id);
}

void Texture::unload()
{
	SDL_FreeSurface(image);
}
