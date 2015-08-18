#pragma once
#include "common.h"

class IEventHandler
{
public:
	// Handels an input event
	virtual void handel(SDL_Event const& sdlevent) = 0;
};

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

/******************************************************************************
** WindowResizeHandler
******************************************************************************/

class WindowResizeHandler : public IEventHandler
{
private:
	Window* const _window;

public:
	WindowResizeHandler(Window* window)
		: _window(window)
	{ }

	virtual void handel(SDL_Event const& sdlevent);
};