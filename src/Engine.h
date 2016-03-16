#pragma once
#include "common.h"

#include "config.h"
#include "assets/assets.h"

/******************************************************************************
** Forward Declare Engine Parts
******************************************************************************/

struct WindowDescription;
struct Window;

class IGame;
class IScene;

/******************************************************************************
** Engine
******************************************************************************/

/* T:
Describes the engine singleton root object.
*/

class Engine
{
private:
	EXPORTED static void _init();
	EXPORTED void _init_setGame(IGame* game);
	EXPORTED void _main();

	Engine();
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;

	friend int main(int argc, char** argv);

	IGame* _game;
	Config* _config;

	Assets* _assets;
private:
	std::vector<Window> _windows_window;
	std::vector<WindowDescription> _windows_windowDescription;
	std::map<std::string, unsigned int> _windows_byName;

public:
	/* T:
	This section describes the shared API and variables of the engine object.

	Starting with access to the engine singleton and the current game object.
	*/
	EXPORTED static Engine& instance();
	inline IGame* game() { return _game; }
	inline Config* config() { return _config; }

	inline Assets* assets() { return _assets; }

	/* T:
	This section describes management of the windows, a top level engine responsibility.

	* `windowGet` will retrieve a window by name.
	* `windowUpdate` will inspect the game for the windows it is requesting and do the following:
	  * Close windows with out-dated names.
	  * Reinit windows with different flags.
	  * Create new windows.
	  * Move or resize windows that have changed size or position.

	TODO: Add events into window struct.
	TODO: Refactor this to a class
	*/
	EXPORTED Window* windowGet(std::string window_name);
	EXPORTED void windowUpdate();

protected:
	/* T:
	This section describes the internal methods used by the engine, almost all are virtual for
	customization and experimentation usage.

	`loop_` describes the loop features. `loop_main` is the entire loop, `loop_core` is the primary
	inner loop besides events. `loop_event` is the event handling portion of the inner loop.

	`sdl_` describes the sdl wrapping features.

	`init_` describes the one time init features. `quit_` is init in reverse.
	*/

	virtual void loop_main();
	virtual void loop_core();
	virtual void loop_event();
	
	virtual bool sdl_updateWindow(WindowDescription* desc_new, WindowDescription* desc_old, Window* window);

	virtual void init();
	virtual void init_sdl();
	virtual void init_glew();

	virtual void setup();

	virtual void quit();
	virtual void quit_sdl();
};

/******************************************************************************
** Engine Parts
******************************************************************************/

enum CONFIG_GFX_LIB
{
	CONFIG_GFX_LIB_OGL2 = 2,
	CONFIG_GFX_LIB_OGL3 = 3,
	CONFIG_GFX_LIB_OGL4 = 4,
	CONFIG_GFX_LIB_VULKAN = 16
};

/* T:
Describes what windows should be created for the game.
*/
struct WindowDescription
{
	std::string name;

	Uint32 sdl_flags;
	CONFIG_GFX_LIB gl_library_type;

	std::string title;
	int width;
	int height;
	int x;
	int y;
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

/******************************************************************************
** Engine Parts Include
******************************************************************************/

/* T:
The following include order is carefully arranged due to recursive class refrences
*/

#include "IGame.h"

class IScene {}; // Placeholder

/******************************************************************************
** Engine Macros
******************************************************************************/

#define ASSETPATH(path) Engine::instance().assets()->AssetPathToSystemPath(std::string((path)))