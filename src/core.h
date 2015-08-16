#pragma once

#include "common.h"
#include "asset.h"
#include "shader.h"

/* T:
Describes the contents of both driver.cpp (containing main) and app.cpp (containing primary 
user code).

This file describes the global application structures, as well as how the driver interacts with
the user's application code.
*/

/******************************************************************************
** Simple Wrappers
******************************************************************************/

/* T:
Holds the SDL variables for a window and it's opengl context. This can be used to manage them via
SDL.
*/
struct Window
{
	SDL_Window* sdlwindow;
	SDL_GLContext glcontext;
};

/******************************************************************************
** User Applicaton / Opengl
******************************************************************************/

/* T:
Describes the interface used by the driver.

`new_App` and `del_App` are used by the driver to allocate and delete the app.
*/
class IApp
{
public:
	virtual void init(Window* window) = 0;
	virtual void draw() = 0;
	virtual void event(SDL_Event* event) = 0;
};

// Defined in `app.cpp`
IApp* new_App();
void del_App(IApp* app);