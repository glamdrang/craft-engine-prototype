#include "common.h"

#include "Engine.h"

#include "SDL2\SDL_syswm.h" // For Vulkan access

/******************************************************************************
** Lifecycle
******************************************************************************/

static Engine* __engine_instance;

void Engine::_init()
{
	__engine_instance = new Engine();
	__engine_instance->init();
}

void Engine::setup()
{
	std::vector<std::string> paths;
	this->_config = make_config(paths);

	_assets = new Assets();

	try
	{
		std::string asset_path = this->_config->rootFile_rootNode["assets"]["hives"][0]["root"].as<std::string>();
		_assets->AddHive(path::join(path::dir(this->_config->rootFile_path), asset_path));
	}
	catch (YAML::Exception)
	{

	}
}

void Engine::_init_setGame(IGame* game)
{
	this->_game = game;

	this->setup();

	this->windowUpdate();
	game->init();
}

void Engine::_main()
{
	this->loop_main();
}

Engine::Engine()
{
	this->_types = new TypesEcs();
}

Engine& Engine::instance()
{
	return *__engine_instance;
}

/******************************************************************************
** Engine API
******************************************************************************/

Window* Engine::windowGet(std::string window_name)
{
	auto it = this->_windows_byName.find(window_name);
	if (it == this->_windows_byName.end())
		return nullptr;
	else
	{
		unsigned int i = (*it).second;
		if (i >= _windows_window.size())
			return nullptr;
		return &_windows_window[i];
	}
}

void Engine::windowUpdate()
{
	std::set<std::string> toClose;
	for (auto it : _windows_byName)
		toClose.insert(it.first);

	std::vector<Window> new_windows_window;
	std::vector<WindowDescription> new_windows_windowDescription;
	std::map<std::string, unsigned int> new_windows_byName;
	unsigned int index_next = 0;

	for (WindowDescription desc : game()->windows())
	{
		// Check to see if desc in existing set
		auto it_desc_toClose = toClose.find(desc.name);

		Window window;

		// We are updating the window
		if (it_desc_toClose != toClose.end())
		{
			// Erase from close set
			toClose.erase(it_desc_toClose);

			// Retrieve existing description to update from
			WindowDescription* desc_existing = &_windows_windowDescription[_windows_byName[desc.name]];

			// Execute update
			this->sdl_updateWindow(&desc, desc_existing, &window);
		}
		// Creating a new window
		else
		{
			// Execute update
			this->sdl_updateWindow(&desc, 0, &window);
		}

		// Update information
		new_windows_byName[desc.name] = index_next;
		new_windows_window.push_back(window);
		new_windows_windowDescription.push_back(desc);
		index_next++;
	}

	for (auto name : toClose)
	{
		// Retrieve information
		unsigned int name_index = _windows_byName[name];
		Window* window = &_windows_window[name_index];
		WindowDescription* desc = &_windows_windowDescription[name_index];

		// Execute update
		this->sdl_updateWindow(0, desc, window);
	}

	// Set the new data structures
	_windows_window = new_windows_window;
	_windows_windowDescription = new_windows_windowDescription;
	_windows_byName = new_windows_byName;
}

/******************************************************************************
** Engine Internals
******************************************************************************/

/******************************************************************************
** Engine Internals - Loop
******************************************************************************/

void Engine::loop_main()
{
	SDL_Event event;

	// Turn on VSync
	SDL_GL_SetSwapInterval(1);

	bool running = true;
	while (running)
	{
		/* T:
		Here is our main loop.

		*/

/*[0]*/ game()->draw();

/*[2]*/ while (SDL_PollEvent(&event))
		{
/*[3]*/		if (event.type == SDL_QUIT
				|| (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
			{
				running = false;
			}
/*[4]*/ 	game()->event(event);
		}
	}
}

void Engine::loop_core()
{

}

void Engine::loop_event()
{

}

/******************************************************************************
** Engine Internals - SDL
******************************************************************************/

bool Engine::sdl_updateWindow(WindowDescription* desc_new, WindowDescription* desc_old, Window* window)
{
	bool willRemove = false;
	bool willCreate = false;
	bool hasBoth = false;
	if (desc_new == nullptr)
	{
		willRemove = true;
	}
	else
	{
		hasBoth = (desc_old != nullptr);
		// We will create if we don't have an old description...
		willCreate = !hasBoth
			// or the descriptions' sdl/gl_lib flags mismatch...
			|| (desc_new->sdl_flags != desc_old->sdl_flags)
			|| (desc_new->gl_library_type != desc_old->gl_library_type)
			// or the description wants OpenGL and it hasn't been created...
			|| (desc_new->sdl_flags & SDL_WINDOW_OPENGL && window->glContext == nullptr)
			// or the sdl window is null.
			|| (window->sdlWindow == nullptr);
	}

	if (willRemove || willCreate)
	{ 
		// Remove each part of the window if it exists, in order
		if (window->glContext != nullptr)
			SDL_GL_DeleteContext(window->glContext);
		if (window->sdlWindow != nullptr)
			SDL_DestroyWindow(window->sdlWindow);

		window->glContext = nullptr;
		window->sdlWindow = nullptr;
	}

	if (willCreate)
	{
		// TODO better descign description struct to prevent this.
		bool wantsOpengl =
			desc_new->gl_library_type >= CONFIG_GFX_LIB_OGL2
			&& desc_new->gl_library_type <= CONFIG_GFX_LIB_OGL4;
		
		int sdl_flags = desc_new->sdl_flags;

		if (wantsOpengl)
			sdl_flags = sdl_flags | SDL_WINDOW_OPENGL;

		// Create a new window, we should have a clean window struct now too.
		window->sdlWindow = SDL_CreateWindow(
			desc_new->title.c_str(),
			(desc_new->x != -1) ? desc_new->x : SDL_WINDOWPOS_CENTERED,
			(desc_new->y != -1) ? desc_new->y : SDL_WINDOWPOS_CENTERED,
			desc_new->width,
			desc_new->height,
			sdl_flags);

		if (window->sdlWindow == nullptr)
		{
			std::cerr << "SDL Error [Window - Create]: " << SDL_GetError() << std::endl;
			return false;
		}

		if (wantsOpengl)
		{
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

			switch (desc_new->gl_library_type)
			{
			case CONFIG_GFX_LIB_OGL4:
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
				break;

			case CONFIG_GFX_LIB_OGL3:
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
				break;

			case CONFIG_GFX_LIB_OGL2:
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
				break;

			default:
				std::cerr << "SDL Error [Window - Lib Init]: " << SDL_GetError() << std::endl;
				window->sdlWindow = 0;
				return false;
			}

			// Double buffer with 24 bit Z
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

			// Steroscopic, will be quad buffered if double buffer is enabled.
			// See here for glDrawBuffer: https://www.opengl.org/sdk/docs/man/html/glDrawBuffer.xhtml
			//SDL_GL_SetAttribute(SDL_GL_STEREO, 1);

			window->glContext = SDL_GL_CreateContext(window->sdlWindow);

			if (window->glContext == nullptr)
			{
				std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
				SDL_DestroyWindow(window->sdlWindow);
				window->sdlWindow = nullptr;
				return false;
			}

			// Set Default Viewport
			glViewport(0, 0, desc_new->width, desc_new->height);
		}
		else if (desc_new->gl_library_type == CONFIG_GFX_LIB_VULKAN)
		{
			SDL_SysWMinfo info;
			SDL_VERSION(&info.version);

			if (!SDL_GetWindowWMInfo(window->sdlWindow, &info))
			{
				std::cerr << "SDL Window manager info failed: " << SDL_GetError() << std::endl;
				SDL_DestroyWindow(window->sdlWindow);
				window->sdlWindow = nullptr;
				return false;
			}

			switch (info.subsystem) {
				case SDL_SYSWM_WINDOWS:
					// https://www.reddit.com/r/vulkan/comments/469v49/is_anyone_working_on_getting_vulkan_running_with/
					// Put it here
					break;
				default:
					std::cerr << "SDL cannot init Vulkan on this platform." << std::endl;
					SDL_DestroyWindow(window->sdlWindow);
					window->sdlWindow = nullptr;
					return false;
			}
		}
		else
		{
			std::cerr << "SDL - Unknown GL Library (" << desc_new->gl_library_type << ")" << std::endl;
		}
	}

	if (hasBoth
		&& (desc_new->x != desc_old->x
			|| desc_new->y != desc_old->y))
	{
		SDL_SetWindowPosition(window->sdlWindow,
			(desc_new->x != -1) ? desc_new->x : SDL_WINDOWPOS_CENTERED,
			(desc_new->y != -1) ? desc_new->y : SDL_WINDOWPOS_CENTERED);
	}

	if (hasBoth
		&& (desc_new->width != desc_old->width
			|| desc_new->height != desc_old->height))
	{
		SDL_SetWindowPosition(window->sdlWindow,
			desc_new->width,
			desc_new->height);
	}

	return true;
}

/******************************************************************************
** Engine Internals - Init / Quit
******************************************************************************/

void Engine::init()
{
	init_sdl();
	init_glew();
}

void Engine::quit()
{
	quit_sdl();
}

void Engine::init_sdl()
{
	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Unable to initialize SDL!";
		exit(1);
	}
	if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF))
	{
		std::cerr << "Unable to initialize SDL IMG!";
		exit(1);
	}
	if (TTF_Init() != 0)
	{
		std::cerr << "Unable to initialize SDL TTF!";
		exit(1);
	}
}

void Engine::quit_sdl()
{
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

void Engine::init_glew()
{
	/* T:
	To init glew we need an OpenGL context. Any context, so we cheat by creating and then deleting
	a window.
	*/
	WindowDescription wnd_desc;
	wnd_desc.name = "ENGINE:Engine.cpp:init_glew";
	wnd_desc.title = "[[[SPECIAL - Engine.cpp:init_glew]]]";
	wnd_desc.sdl_flags = SDL_WINDOW_HIDDEN;
	wnd_desc.gl_library_type = CONFIG_GFX_LIB_OGL2;
	wnd_desc.width = 1024;
	wnd_desc.height = 768;
	wnd_desc.x = -1;
	wnd_desc.y = -1;

	Window wnd;
	if (!this->sdl_updateWindow(&wnd_desc, nullptr, &wnd)) // Create Window
	{
		std::cerr << "GLEW failed to build temp window for init." << std::endl;
		exit(1);
	}

	// BEGIN: Glew Init
	glewExperimental = GL_TRUE;
	GLenum ret = glewInit();

	if (GLEW_OK != ret)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(ret) << std::endl;
		exit(1);
	}

	std::cerr << "GLEW Status: GL 4.4: " << (bool)GL_VERSION_4_4 << std::endl;
	std::cerr << "GLEW Status: GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	// END: Glew Init

	if (!this->sdl_updateWindow(nullptr, &wnd_desc, &wnd)) // Delete Window
	{
		std::cerr << "GLEW failed to close temp window for init." << std::endl;
		exit(1);
	}
}