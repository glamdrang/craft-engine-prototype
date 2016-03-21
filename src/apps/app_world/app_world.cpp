#include "common.h"
#include "core.h"
#include "features/handlers.h"
#include "features/StandardCamera.h"

#include "world.h"

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
	ExampleWorld_ECS world;

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

	ecs::EntityId e0 = world.e();
	
	std::cerr << "world tests:" << std::endl
		<< "    managers                            == 2:   " << world.managerCount() << std::endl
		<< "    entities                            == 1:   " << world.entityCount() << std::endl
		<< "    world.e_has(e0)                     == 1:   " << world.e_has(e0) << std::endl
		<< "    world.c_has<CTransform>(e0)         == 0:   " << world.c_has<CTransform>(e0) << std::endl
		//<< "    world.c_has<CPhysics>(e0): " << world.c_has<CPhysics>(e0) << std::endl // Should error
		;

	CTransform t0_ = {};
	t0_.pos.x = 1.1f;
	world.c_set(e0, t0_);
	t0_.pos.x = 5.5f; // Should not update
	t0_ = world.c_get<CTransform>(e0);

	auto t0a = &world.c<CTransform>(e0);
	auto& t0b = world.c<CTransform>(e0);
	auto t0c = world.c<CTransform>(e0);

	std::cerr << "    **** 0" << std::endl
		<< "    world.c_has<CTransform>(e0)         == 1:   " << world.c_has<CTransform>(e0) << std::endl
		<< "    world.c<CTransform>(e0).pos.x       == 1.1: " << (float)world.c<CTransform>(e0).pos.x << std::endl
		<< "    world.c_get<CTransform>(e0).pos.x   == 1.1: " << (float)world.c_get<CTransform>(e0).pos.x << std::endl
		<< "    t0_.pos.x                           == 1.1: " << (float)t0_.pos.x << std::endl
		;

	t0a->pos.x = 2.2f;

	std::cerr << "    **** 1 `auto x = &w.c`" << std::endl
		<< "    world.c<CTransform>(e0).pos.x       == 2.2: " << (float)world.c<CTransform>(e0).pos.x << std::endl
		;

	t0b.pos.x = 3.3f;

	std::cerr << "    **** 2 `auto x = &w.c`" << std::endl
		<< "    world.c<CTransform>(e0).pos.x       == 3.3: " << (float)world.c<CTransform>(e0).pos.x << std::endl
		;

	t0c.pos.x = 4.4f;

	std::cerr << "    **** 3 `auto x = &w.c`" << std::endl
		<< "    world.c<CTransform>(e0).pos.x       != 4.4: " << (float)world.c<CTransform>(e0).pos.x << std::endl
		;

	world.c_rem<CTransform>(e0);
	world.e_rem(e0);

	std::cerr << "    **** 4" << std::endl
		<< "    world.c_has<CTransform>(e0)         == 0:   " << world.c_has<CTransform>(e0) << std::endl
		<< "    entities                            == 0:   " << world.entityCount() << std::endl
		;
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

	_shader = LoadShader(ASSETPATH("shaders\\simple.vert.glsl").c_str(), ASSETPATH("shaders\\simple.frag.glsl").c_str());

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