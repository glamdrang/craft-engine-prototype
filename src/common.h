#pragma once

// Our includes
#include "config.h"

// C++
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

// SDL
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// GL
#include <GL/glew.h> // Must be before general GL include
#include <GL/GL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

// Assest Importer
#include <assimp/types.h>

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

/* T:
SDL event handler wrapped in an interface (rather than a callback).
*/
class IEventHandler
{
public:
	// Handels an input event
	virtual void handel(SDL_Event const& sdlevent) = 0;
};
