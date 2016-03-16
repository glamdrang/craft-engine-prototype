#pragma once
#include "../common.h"

#include "../gfx/shader.h"
#include "InputText.h"

class Console
{
	const int _consoleLength = 2500;
	int _consoleCount;
	std::string _consoleLog;

	SDL_Surface* _surface;
	TTF_Font* _font;
	int _font_height;

	GLuint _gl_tex_id;
	GLuint _gl_shader;
	GLuint _gl_vao, _gl_vao_vbo, _gl_vao_ibo;

	const std::string
		_vert_shader = "shaders\\console.vert.glsl",
		_frag_shader = "shaders\\console.frag.glsl";

	SDL_Rect _rect;
public:
	InputText InputText;

	EXPORTED Console();

	EXPORTED void append_text(std::string const &t);

	EXPORTED void set_window(int width, int height);

	EXPORTED void update();
	EXPORTED void draw();
};
