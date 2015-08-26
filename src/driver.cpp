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
	app->window_spec(sdl_flags, width, height, x, y);

	// Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
	window->sdlwindow = SDL_CreateWindow(
		"SDL2/OpenGL Demo",
		(x != -1) ? x : SDL_WINDOWPOS_CENTERED, (y != -1) ? y : SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | sdl_flags);

	if (window->sdlwindow == 0)
	{
		std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
		delete window;
		return 0;
	}

	// Request opengl 4.4 context.
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

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
		
		First we have the app draw the scene (this replaces the current back buffer). Then we swap
		buffers so the back buffer is displayed and the front buffer becomes the back buffer (to be
		drawn over in the next call).
		
		Next we poll for system events given to our application or window. We have a hardcoded quit
		for window or application quit events to not run the next fram. We forward all events to
		the app.

		*/
		app->draw();
		SDL_GL_SwapWindow(win->sdlwindow);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT
				|| (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
			{
				running = false;
			}
			app->event(event);
		}
	}
}

// For macros: http://sourceforge.net/p/predef/wiki/OperatingSystems/
// Remove `0 && ` to make a pure graphical app on windows (no terminal)
#if 0 && defined(_WIN32)
#include <Windows.h>
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	LPWSTR *szArgList;
	int argc;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	char** argv = new char*[argc + 1];
	argv[argc] = 0;
	for (int i = 0; i < argc; i++)
	{
		int length = WideCharToMultiByte(cp, 0, sampleObject->a, -1, 0, 0, NULL, NULL);
		argv[i] = new char[length];
		WideCharToMultiByte(cp, 0, sampleObject->a, -1, argv[i], length, NULL, NULL);
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