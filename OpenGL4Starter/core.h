#pragma once

#include "common.h"
#include "util\asset.h"
#include "util\shader.h"

/******************************************************************************
** Simple Wrappers
******************************************************************************/

struct Window
{
	SDL_Window* sdlwindow;
	SDL_GLContext glcontext;
};

/******************************************************************************
** User Applicaton / Opengl
******************************************************************************/

// Declared in a eperate namespace so the driver can use them and they can be
// defined in multiple files and chosen via compler switch.
class IApp
{
public:
	virtual void init(Window* window) = 0;
	virtual void draw() = 0;
	virtual void event(SDL_Event* event) = 0;
};

IApp* new_App();
void del_App(IApp* app);