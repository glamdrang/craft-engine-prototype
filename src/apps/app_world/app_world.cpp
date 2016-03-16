#include "common.h"
#include "core.h"
#include "features/handlers.h"
#include "features/StandardCamera.h"

class WorldGame : public IGame
{
	Window* _window;
	StandardCamera _camera;

	// TODO(Mason): Replace these with the full example scene manager
	IAsset* _asset; // The one asset in our scene
	GLint _shader; // The one shader in our scene

	IInputTextHandler *_input_text;
	std::vector<IInputHandler*> _input_handlers;

	std::vector<WindowDescription> _windows;

public:
	virtual std::vector<WindowDescription>& windows();

	virtual void init();
	virtual void draw();
	virtual void event(SDL_Event& event);

private:
	WorldGame();

	friend int main(int argc, char** argv);
};

WorldGame::WorldGame()
{
	WindowDescription desc;
	desc.name = "example:main";
	desc.title = "Simple App";
	desc.gl_library_type = CONFIG_GFX_LIB_OGL4;
	desc.sdl_flags = SDL_WINDOW_RESIZABLE;
	desc.width = 1024;
	desc.height = 768;
	desc.x = -1;
	desc.y = -1;

	_windows.push_back(desc);
}

std::vector<WindowDescription>& WorldGame::windows()
{
	return _windows;
}

void WorldGame::init()
{
	_window = Engine::instance().windowGet("example:main");

	_input_handlers.push_back((IInputHandler*) new WindowResizeHandler(_window));
	_input_handlers.push_back((IInputHandler*)&_camera);

	_shader = LoadShader("..\\assets\\shaders\\simple.vert.glsl", "..\\assets\\shaders\\simple.frag.glsl");

	//_world_texture = LoadTexture();
}

void WorldGame::draw()
{
	_camera.update();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(_shader);

	glUniformMatrix4fv(glGetUniformLocation(_shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(_camera.matrix_projection));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(_camera.matrix_view));


	AssetShaderUniforms uniforms;
	uniforms.texture_diffuse = glGetUniformLocation(_shader, "texDiffuse");
	glUniform1i(uniforms.texture_diffuse, 1);

	uniforms.color_diffuse = glGetUniformLocation(_shader, "colorDiffuse");

	/* T:
	Here we render our scene using the above shader. For now it's a single asset. The location of
	the asset in the scene is set using the `modelMatrix` so the shader knows where to place it.

	*/
	glm::mat4 modelMat = glm::scale(glm::mat4(1.0), glm::vec3(0.01, 0.01, 0.01));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMat));

	//_asset->gl_render(uniforms);

	/* T:
	Here we reset the OpenGL states back to their defualt.
	*/
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void WorldGame::event(SDL_Event& event)
{
	if (_input_text != 0 && _input_text->is_text_sdl_event_handler_active())
	{
		_input_text->handle(event);
	}
	else
	{
		for (auto it = _input_handlers.begin(); it != _input_handlers.end(); it++)
		{
			(*it)->handle(event);
		}
	}
}

typedef WorldGame MainGame;

#include "main.inc"