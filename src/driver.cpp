#include "core.h"

/******************************************************************************
** Library Init/Quit Helpers
******************************************************************************/

void init_sdl()
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

void quit_sdl()
{
	SDL_Quit();
	IMG_Quit();
}

void init_glew()
{
	// We push the limit here on some drivers
	glewExperimental = GL_TRUE;
	GLenum ret = glewInit();

	if (GLEW_OK != ret)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(ret) << std::endl;
		exit(1);
	}

	std::cerr << "GLEW Status: GL 4.4: " << (bool)GL_VERSION_4_4 << std::endl;
	std::cerr << "GLEW Status: GLEW " << glewGetString(GLEW_VERSION) << std::endl;
}

/******************************************************************************
** Misc Helpers
******************************************************************************/

Window* new_sdlWindow(IApp* app)
{
	Window* window = new Window();

	Uint32 sdl_flags;
	int width, height, x, y;
	std::string name;
	app->window_spec(sdl_flags, width, height, x, y, name);

	// Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
	window->sdlwindow = SDL_CreateWindow(
		name.c_str(),
		(x != -1) ? x : SDL_WINDOWPOS_CENTERED, (y != -1) ? y : SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | sdl_flags);

	if (window->sdlwindow == 0)
	{
		std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
		delete window;
		return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// OpenGL request
#if OPTION_OPENGL_VERSION == 4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#elif OPTION_OPENGL_VERSION == 3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
#error Invalid opengl version specified (define OPTION_OPENGL_VERSION)
#endif

	// Double buffer with 24 bit Z
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Steroscopic, will be quad buffered if double buffer is enabled.
	// See here for glDrawBuffer: https://www.opengl.org/sdk/docs/man/html/glDrawBuffer.xhtml
	//SDL_GL_SetAttribute(SDL_GL_STEREO, 1);

	// Create an OpenGL context associated with the window.
	window->glcontext = SDL_GL_CreateContext(window->sdlwindow);
	glViewport(0, 0, 1024, 800);

	return window;
}

void del_sdlWindow(Window* window)
{
	SDL_GL_DeleteContext(window->glcontext);
	SDL_DestroyWindow(window->sdlwindow);

	delete window;
}

/******************************************************************************
** Main
******************************************************************************/

void mainLoop(IApp* app, Window* win)
{
	SDL_Event event;

	// Turn on VSync
	SDL_GL_SetSwapInterval(1);

	bool running = true;
	while (running)
	{
		/* T:
		Here is our main loop.
		
		First [0] we have the app draw the scene (this replaces the current back buffer). Then [1]
		we swap buffers so the back buffer is displayed and the front buffer becomes the back 
		buffer (to be drawn over in the next call).
		
		Next [2] we poll for system events given to our application or window. We have [3] a 
		hardcoded quit for window or application (ESC) quit events to not run the next frame. We 
		forward [4] all events to the app (including quit).

		*/
		
/*[0]*/ app->draw();
/*[1]*/ SDL_GL_SwapWindow(win->sdlwindow);

/*[2]*/ while (SDL_PollEvent(&event))
		{
/*[3]*/		if (event.type == SDL_QUIT
				|| (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
			{
				running = false;
			}
/*[4]*/ 	app->event(event);
		}
	}
}

// For macros: http://sourceforge.net/p/predef/wiki/OperatingSystems/
// Remove `0 && ` to make a pure graphical app on windows (no terminal)
#if defined(_WIN32) && OPTION_MAIN_WINDOWING
#include <Windows.h>
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	LPWSTR* argvw;
	int argc;

	argvw = CommandLineToArgvW(GetCommandLineW(), &argc);

	// Convert argvw into argv
	char** argv = new char*[argc + 1];
	argv[argc] = 0;
	for (int i = 0; i < argc; i++)
	{
		int length = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, argvw[i], -1, NULL, 0, NULL, NULL);
		argv[i] = new char[length];
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, argvw[i], -1, argv[i], length, NULL, NULL);
	}
#else
int main(int argc, char** argv)
{
#endif
	Window* win;
	IApp* app = new_App();

	// Init libraries
	init_sdl();
	win = new_sdlWindow(app); // Also inits the GLContext

	init_glew(); // Requires GL context to inited by window.

	// User code
	app->init(win);

	// Main Loop
	mainLoop(app, win);

	// Quit / cleanup functions
	del_App(app);
	del_sdlWindow(win);
	quit_sdl();
}