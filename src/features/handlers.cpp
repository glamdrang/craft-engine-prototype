#include "common.h"
#include "handlers.h"

/******************************************************************************
** WindowResizeHandler
******************************************************************************/

WindowResizeHandler::WindowResizeHandler(Window* window)
	: _window(window)
{ }

void WindowResizeHandler::handle(SDL_Event const& sdlevent)
{
	if (sdlevent.type == SDL_WINDOWEVENT
		&& sdlevent.window.event == SDL_WINDOWEVENT_RESIZED
		&& sdlevent.window.windowID == SDL_GetWindowID(_window->sdlWindow))
	{
		std::cerr << "Window resized to " << sdlevent.window.data1 << "x" << sdlevent.window.data2 << std::endl;

		glViewport(0, 0, sdlevent.window.data1, sdlevent.window.data2);
	}
}
