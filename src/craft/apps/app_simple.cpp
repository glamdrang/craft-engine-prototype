#include "craft/common.h"
#include "craft/core.h"

class ExampleGame : public IGame
{
	Window* _window;
	StandardCamera _camera;

	Console _console;

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
	ExampleGame();

	friend int main(int argc, char** argv);
};

ExampleGame::ExampleGame()
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

std::vector<WindowDescription>& ExampleGame::windows()
{
	return _windows;
}

void ExampleGame::init()
{
	_window = Engine::instance().windowGet("example:main");

	int width, height;
	SDL_GetWindowSize(_window->sdlWindow, &width, &height);

	_console.set_window(width, height);

	_input_text = &_console.InputText;
	_input_handlers.push_back((IInputHandler*) &_console.InputText);
	_input_handlers.push_back((IInputHandler*) new WindowResizeHandler(_window));
	_input_handlers.push_back((IInputHandler*) &_camera);

	/* T:
	Here we load our assets before we render anything. In most applications we would use an
	asynchronous loading system which would do some portion of needed GL calls every frame before
	drawing.

	LoadShader can be found in shader.h/cpp; it returns an opengl ShaderProgram
	AssetFromFile (and the IAsset interface) can be found in asset.h/cpp
	*/
	_shader = LoadShader(ASSETPATH("shaders\\simple.vert.glsl").c_str(), ASSETPATH("shaders\\simple.frag.glsl").c_str());

	_asset = AssetFromFile(ASSETPATH("Banana.obj").c_str());

	_asset->load(); // Load from disk into main memory
	_asset->gl_load(); // Load from main memory to gpu memory
	_asset->unload(); // Unload from main memory
}

void ExampleGame::draw()
{
	/* T:
	Update our scene
	*/
	_camera.update();
	_console.update();

	/* T:
	Here we set up the rendering state for this call. We do this every frame because it may have been
	removed by other OpenGL calls which are unknown to this rendering loop (however we can hope they
	are at least set back to their defaults).

	* GL_CULL_FACE: enables face culling, which by defualt will cull the back face, as determined by
	    winding order. See: https://www.opengl.org/wiki/Face_Culling
	* GL_DEPTH_TEST: enables depth testing, which is used to determine which overlapping object is
	    drawn to the color buffer. This is done using the depth buffer to determine the z location
	    of each pixel. See: https://www.opengl.org/wiki/Depth_Test
	*/
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


	/* T:
	Here we clear the buffer of it's previous contents. We clear the color buffer (what is actually 
	seen) and the depth buffer (the z location of the pixel's color relative tot he camera).
	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/* T:
	Here we configure our shader we compiled earlier.

	First we set it as the current shader. Like all opengl state this is global to the context. To
	modify shader uniform variables (variables which do not change per core of the shader) we use
	the `glUniform*` family of functions (which come in both pointer form, ending in v and direct
	form). To get the id of the variable to change we use `glGetUniformLocation`, which in more
	advanced systems would *not* be called every frame (as it's relatively expensive).

	We set the projection and view matrix to those supplied by the camera.

	* projMat: Our projection matrix which is built using field of view, aspect ratio and near/far 
	    planes to map the view space vertex to OpenGL's display cube (1.0 to -1.0 on all 3 
	    dimensions). See: http://www.songho.ca/opengl/gl_projectionmatrix.html

	* viewMat: This matrix maps world space to view space. Specifically so the camera can be thought
	    of as looking down -z at the origin (so we can apply the project transform). We use the
	    lookAt utility function to simplyfy this.

	For information on view, model, and world space see:
	http://www.codinglabs.net/article_world_view_projection_matrix.aspx
	*/
	glUseProgram(_shader);

	glUniformMatrix4fv(glGetUniformLocation(_shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(_camera.matrix_projection));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(_camera.matrix_view));


	/* T:

	Here We grab other required shader uniforms and place them into a structure so our asset knows
	what they are in the current shader. We partially rely on a standard for textures. Where the
	diffuse texture is assumed to be at textrue unit 1 (e.g. `GL_TEXTURE0 + 1`) and if the asset 
	changes that we assume it will change it back when done.

	The shader uniform system is rather robust. It returns a -1 for any variable the shader doesn't
	have (or on name error), and attempting to bind to uniform -1 has no effect. This is because
	drivers aggressively optimize shaders and may remove variables they deem unneccessary.

	Generally our scene-graph (currently non-exisitent) would cull models which wouldn't end up
	being rendered.
	*/
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

	_asset->gl_render(uniforms);

	/* T:
	Here we reset the OpenGL states back to their defualt.
	*/
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	/* T:
	Here we render the console ontop of the window.
	*/
	_console.draw();

	/* T:
	Finally we use SDL to swap our render buffers.
	*/
	SDL_GL_SwapWindow(_window->sdlWindow);
}

void ExampleGame::event(SDL_Event& event)
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

typedef ExampleGame MainGame;
#include "craft/main.inc"