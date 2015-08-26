#pragma once

#include "common.h"

#include "event_input.h"

#include "camera.h"
#include "shader.h"
#include "asset.h"

/* T:
Describes the contents of both driver.cpp (containing main) and app.cpp (containing primary 
user code).

This file describes the global application structures, as well as how the driver interacts with
the user's application code.
*/

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
	virtual void window_spec(Uint32& sdl_flags, int& width, int& height, int& x, int& y) = 0;

	virtual void init(Window* window) = 0;
	virtual void draw() = 0;
	virtual void event(SDL_Event& event) = 0;
};

// Defined in `app.cpp`
IApp* new_App();
void del_App(IApp* app);