#include "common.h"
#include "console.h"

#include "Engine.h"

Console::Console()
{
	_consoleLog.append(" Console! \n==--==--==\n");
	_consoleCount = 2;
}

void Console::append_text(std::string const &t)
{
	_consoleLog.append(t);
	_consoleCount += (int)std::count(t.begin(), t.end(), '\n');

	if (t[t.length() - 1] != '\n')
	{
		_consoleLog.append("\n");
		_consoleCount += 1;
	}

	int diff = _consoleCount - _consoleLength;
	if (diff > 0)
	{
		int index = 0;
		while (diff > 0)
		{
			if (_consoleLog[index] == '\n')
			{
				diff--;
			}
			index++;
		}
		_consoleLog.erase(_consoleLog.begin(), _consoleLog.begin() + index);
	}
}

void Console::set_window(int width, int height)
{
	/**************************************************************************
	Surface section
	*/
	if (_surface != nullptr)
	{
		SDL_FreeSurface(_surface);
	}

	std::string font_path = ASSETPATH(".\\VeraMono.ttf");
	_font = TTF_OpenFont(font_path.c_str(), 11);
	if (_font == nullptr)
		return;

	_font_height = TTF_FontLineSkip(_font);

	Uint32 rmask, gmask, bmask, amask;

	/* T:
	Describes SDL interpreatation of images. Which is machine dependent.
	*/
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	_surface = SDL_CreateRGBSurface(0, width, height, 32,
		rmask, gmask, bmask, amask);

	_rect.x = 0;
	_rect.y = 0;
	_rect.w = width;
	_rect.h = height;

	/**************************************************************************
	Opengl section - Textures
	*/
	glGenTextures(1, &_gl_tex_id);

	glBindTexture(GL_TEXTURE_2D, _gl_tex_id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
	glTexImage2D(GL_TEXTURE_2D,
		0, // level
		GL_RGBA, // internalFormat
		_surface->w,
		_surface->h,
		0, // border
		GL_RGBA, // format
		GL_UNSIGNED_BYTE,
		_surface->pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);


	/**************************************************************************
	Opengl section - Shaders
	*/
	_gl_shader = LoadShader(ASSETPATH(_vert_shader).c_str(), ASSETPATH(_frag_shader).c_str());

	glUseProgram(_gl_shader);
	glUniform1i(glGetUniformLocation(_gl_shader, "tex"), 79);

	/**************************************************************************
	Opengl section - Vertex Buffer
	*/
	float vertexData[] = {
		// Vertex Positions
		1.0, 1.0, 0.0, 1.0,
		1.0, -1.0, 0.0, 1.0,
		-1.0, -1.0, 0.0, 1.0,
		-1.0, 1.0, 0.0, 1.0,

		// Vertex 2dPos
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
	};
	size_t vertexPosLength = 4 * 4 * sizeof(float);
	size_t vertexTexLength = 4 * 2 * sizeof(float);

	unsigned int indexData[] = {
		0, 1, 2,
		2, 3, 0
	};
	size_t vertexDataLength = 2 * 3 * sizeof(unsigned int);

	glGenVertexArrays(1, &_gl_vao);
	glBindVertexArray(_gl_vao);

	glGenBuffers(1, &_gl_vao_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _gl_vao_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexPosLength + vertexTexLength, vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)vertexPosLength);

	glGenBuffers(1, &_gl_vao_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _gl_vao_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexDataLength, indexData, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Console::update()
{

}

void Console::draw()
{
	if (!this->InputText.is_text_sdl_event_handler_active())
		return;

	// Update the text
	SDL_FillRect(_surface, nullptr, 0xCC202020);

	SDL_Surface* text;
	SDL_Color color;
	SDL_Rect current_rect;
	current_rect.x = 0;
	current_rect.y = _surface->h;
	current_rect.w = _surface->w;
	current_rect.h = _surface->h;

	color.r = 255;
	color.g = 255;
	color.b = 255;

	current_rect.y -= _font_height;
	text = TTF_RenderText_Solid(_font, (std::string(" >  ") + InputText.get_current_text()).c_str(), color);
	SDL_BlitSurface(text, nullptr, _surface, &current_rect);
	SDL_FreeSurface(text);
	
	current_rect.y -= (_font_height * 3);
	int textIndex = (int)_consoleLog.length() - 2;
	int lastTextIndex = (int)_consoleLog.length() - 2;
	while (current_rect.y >= 0 && textIndex > 1)
	{
		while (_consoleLog[textIndex] != '\n')
		{
			textIndex--;
			if (textIndex < 0)
				break;
		}
		textIndex++;
		text = TTF_RenderText_Solid(_font,
			std::string(
				_consoleLog.begin() + textIndex,
				_consoleLog.begin() + lastTextIndex + 1).c_str(),
			color);
		textIndex -= 2;
		lastTextIndex = textIndex;
		current_rect.y -= _font_height;

		SDL_BlitSurface(text, nullptr, _surface, &current_rect);
		SDL_FreeSurface(text);
	}

	// Render
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Rebuild texture
	glBindTexture(GL_TEXTURE_2D, _gl_tex_id);
	glTexSubImage2D(GL_TEXTURE_2D,
		0, // level
		0, // xoffset
		0, // yoffset
		_surface->w,
		_surface->h,
		GL_RGBA, // format
		GL_UNSIGNED_BYTE,
		_surface->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Do render
	glBindVertexArray(_gl_vao);
	glUseProgram(_gl_shader);

	glActiveTexture(GL_TEXTURE0 + 79);
	glBindTexture(GL_TEXTURE_2D, _gl_tex_id);
	glActiveTexture(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

	glUseProgram(0);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}
