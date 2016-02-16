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

// Spdlog and Fmt format
#include <spdlog\spdlog.h>

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

/******************************************************************************
** Complex Template Utilities
******************************************************************************/

/* T:
See these tutorials:
	* http://eli.thegreenplace.net/2014/variadic-templates-in-c/
*/

/* T: template class _T_elem_type
This is some template magic for accessing arbitrary type arguments from a template using a number.
*/

template <
	size_t k,
	typename... Ts>
class _T_elem_index;


template <
	typename T,
	typename... Ts>
class _T_elem_index<0, T, Ts...> {
public:
	typedef T type;
};


template <
	size_t k,
	typename T,
	typename... Ts>
class _T_elem_index<k, T, Ts...> {
public:
	typedef typename _T_elem_index<k - 1, Ts...>::type type;
};

/* T:
This is some template magic detail class for allowing specialization.
*/
template < class M >
class T_detail
{

};

// elem supporting types, base case
template <
	template <typename...> class M>
class T_detail < M<> >
{
public:
	typedef class M<> type;

	const static bool has_nexttype = false;

	const static size_t elem_count = 0;
};

// elem supporting types, general case
template <
	template <typename, typename...> class M,
	typename T,
	typename... Ts>
class T_detail<M<T, Ts...>>
{
public:
	typedef class M<T, Ts...> type;

	const static bool has_nexttype = true;
	typedef class M<Ts...> next_type;

	const static size_t elem_count = 1 + next_type::elem_count;
};