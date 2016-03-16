#pragma once
#include "../common.h"

/******************************************************************************
** Triangle
******************************************************************************/

typedef unsigned int t_triangle_index;

class Texture
{
public:
	std::string filename;
	SDL_Surface *image;

	GLuint tex_id;

	void load();
	void gl_load();
	void gl_unload();
	void unload();
};

