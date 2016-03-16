// For macros: http://sourceforge.net/p/predef/wiki/OperatingSystems/
// Remove `0 && ` to make a pure graphical app on windows (no terminal)
#if defined(_WIN32)
int main(int argc, char** argv);

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

	main(argc, argv);
}
#endif

int main(int argc, char** argv)
{
	Engine::_init();
	
	IGame* game = new MainGame();

	Engine::instance()._init_setGame(game);

	Engine::instance()._main();
}