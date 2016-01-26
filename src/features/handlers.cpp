#include "handlers.h"

/******************************************************************************
** WindowResizeHandler
******************************************************************************/

void WindowResizeHandler::handel(SDL_Event const& sdlevent)
{
	if (sdlevent.type == SDL_WINDOWEVENT
		&& sdlevent.window.event == SDL_WINDOWEVENT_RESIZED
		&& sdlevent.window.windowID == SDL_GetWindowID(_window->sdlwindow))
	{
		std::cerr << "Window resized to " << sdlevent.window.data1 << "x" << sdlevent.window.data2 << std::endl;

		glViewport(0, 0, sdlevent.window.data1, sdlevent.window.data2);
	}
}
