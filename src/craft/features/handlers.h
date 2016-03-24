#pragma once
#include "../core.h"

/******************************************************************************
** WindowResizeHandler
******************************************************************************/

class WindowResizeHandler : public IEventHandler
{
private:
	Window* const _window;

public:
	EXPORTED WindowResizeHandler(Window* window);

	virtual void handle(SDL_Event const& sdlevent);
};
