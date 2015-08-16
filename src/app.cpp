#include "core.h"

class ExampleApp : public IApp
{
	Window* _window;

	// TODO(Mason): Replace these with the full example scene manager
	IAsset* asset; // The one asset in our scene
	GLint shader; // The one shader in our scene

public:
	virtual void init(Window* win);
	virtual void draw();
	virtual void event(SDL_Event* event);
};

IApp* new_App()
{
	return new ExampleApp();
}

void del_App(IApp* app)
{
	delete (ExampleApp*)app;
}

void ExampleApp::init(Window* win)
{
	_window = win;

	/* T:
	Here we load our assets before we render anything. In most applications we would use an
	asynchronous loading system which would do some portion of needed GL calls every frame before
	drawing.

	LoadShader can be found in shader.h/cpp; it returns an opengl ShaderProgram
	AssetFromFile (and the IAsset interface) can be found in asset.h/cpp
	*/
	shader = LoadShader("..\\assets\\simple.vert.glsl", "..\\assets\\simple.frag.glsl");

	asset = AssetFromFile("..\\assets\\Banana.obj");

	asset->load(); // Load from disk into main memory
	asset->gl_load(); // Load from main memory to gpu memory
	asset->unload(); // Unload from main memory
}

void ExampleApp::draw()
{
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

	* projMat: Our projection matrix which is built using field of view, aspect ratio and near/far 
	    planes to map the view space vertex to OpenGL's display cube (1.0 to -1.0 on all 3 
	    dimensions). See: http://www.songho.ca/opengl/gl_projectionmatrix.html

	* viewMat: This matrix maps world space to view space. Specifically so the camera can be thought
	    of as looking down -z at the origin (so we can apply the project transform). We use the
	    lookAt utility function to simplyfy this.

	* texDiffuse: This binds our diffuse sampler (e.g. color) to texture unit 1 (e.g.
	    `GL_TEXTURE0 + 1`)

	For information on view, model, and world space see:
	http://www.codinglabs.net/article_world_view_projection_matrix.aspx
	*/
	glUseProgram(shader);

	glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMat));

	glm::mat4 viewMat = glm::lookAt(glm::vec3(4.0, 1.0, -4.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMat));

	glUniform1i(glGetUniformLocation(shader, "texDiffuse"), 1);

	/* T:
	Here we render our scene using the above shader. For now it's a single asset. The location of
	the asset in the scene is set using the `modelMat` so the shader knows where to place it.

	Generally our scene-graph (currently non-exisitent) would cull models which wouldn't end up
	being rendered.
	*/

	glm::mat4 modelMat = glm::scale(glm::mat4(1.0), glm::vec3(0.01, 0.01, 0.01));
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMat));
	asset->gl_render();

	/* T:
	Here we reset the OpenGL states back to their defualt.
	*/
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void ExampleApp::event(SDL_Event* event)
{

}