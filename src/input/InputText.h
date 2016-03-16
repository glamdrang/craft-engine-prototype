#pragma once
#include "../common.h"
#include "IInputHandler.h"

class InputText : public IInputTextHandler
{
	std::vector<char> _text;
	bool _active;

public:
	virtual void handle(SDL_Event const& sdlevent);
	virtual bool set_text_sdl_event_handler(bool value);
	virtual bool is_text_sdl_event_handler_active();

	std::string get_current_text();
};
