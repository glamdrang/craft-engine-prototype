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
	size_t _font_height;

	GLuint _gl_tex_id;
	GLuint _gl_shader;
	GLuint _gl_vao, _gl_vao_vbo, _gl_vao_ibo;

	const std::string
		_vert_shader = "..\\assets\\shaders\\console.vert.glsl",
		_frag_shader = "..\\assets\\shaders\\console.frag.glsl";

	SDL_Rect _rect;
public:
	InputText InputText;

	Console();

	void append_text(std::string const &t);

	void set_window(int width, int height);

	void update();
	void draw();
};
