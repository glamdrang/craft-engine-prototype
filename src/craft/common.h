#pragma once
// Precompiled header file.
/* T:
	This is the precompiled header and must be include in all code files to function correctly. It
	is also often included in all header files as well. It contains all shared code in the system.
*/

/******************************************************************************
** Defines
******************************************************************************/

#if defined _WIN32 || defined __CYGWIN__
#ifdef ENGINE_DLL
#ifdef __GNUC__
#define EXPORTED __attribute__ ((dllexport))
#else
#define EXPORTED __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define EXPORTED __attribute__ ((dllimport))
#else
#define EXPORTED __declspec(dllimport)
#endif
#endif
#else
#if __GNUC__ >= 4
#define EXPORTED __attribute__ ((visibility ("default")))
#else
#define EXPORTED
#endif
#endif

/******************************************************************************
** Includes
******************************************************************************/

// C++
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>

// SDL
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

// YAML
#include <yaml-cpp\yaml.h>

// Spdlog and Fmt format
#include <spdlog\spdlog.h>

// Our includes
#include "craft/util/all.h"
#include "craft/ecs/core.h"
#include "craft/types.h"

/******************************************************************************
** Simple Wrappers
******************************************************************************/

/* T:
SDL event handler wrapped in an interface (rather than a callback).
*/
class IEventHandler
{
public:
	// Handels an input event
	virtual void handle(SDL_Event const& sdlevent) = 0;
};

/* T:
Holds the SDL variables for a window and it's opengl context. This can be used to manage them via
SDL.
*/
struct Window
{
	Window() : sdlWindow(nullptr), glContext(nullptr) { }

	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
};