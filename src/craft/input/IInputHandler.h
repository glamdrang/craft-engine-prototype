#pragma once
#include "../common.h"

class IInputHandler : public IEventHandler
{
public:
};

class IInputTextHandler : public IInputHandler
{
public:
	// Sets this text handler active
	virtual bool set_text_sdl_event_handler(bool value) = 0;

	// Determins if this text handler is active
	virtual bool is_text_sdl_event_handler_active() = 0;
};
