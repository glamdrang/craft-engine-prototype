#pragma once

#include "common.h"

#include "input/IInputHandler.h"
#include "input/console.h"

#include "gfx/camera.h"
#include "gfx/shader.h"
#include "gfx/asset.h"

/* T:
Describes the contents of both driver.cpp (containing main) and various app_*.cpp files (containing
primary user code).

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
	virtual void window_spec(Uint32& sdl_flags, int& width, int& height, int& x, int& y, std::string& name) = 0;

	virtual void init(Window* window) = 0;
	virtual void draw() = 0;
	virtual void event(SDL_Event& event) = 0;

	ICompositeType* type_root;
};

// Defined in `app.cpp`
IApp* new_App();
void del_App(IApp* app);