#pragma once
#include "../common.h"

class IInputHandler : public IEventHandler
{
public:
};

class IInputTextHandler : public IInputHandler
{
public:
	// Determins if text handler is active
	virtual bool is_text_sdl_event_handler_active() = 0;
};
