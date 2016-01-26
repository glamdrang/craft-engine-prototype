#pragma once
#include "../common.h"

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
