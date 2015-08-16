#include "core.h"

class ExampleApp : public IApp
{
	IAsset* asset;
	GLint shader;

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
	shader = LoadShader("H:\\Work\\ogl4-starter\\assets\\simple.vert.glsl", "H:\\Work\\ogl4-starter\\assets\\simple.frag.glsl");

	asset = assetFromFile("H:\\Work\\ogl4-starter\\assets\\Banana.obj");
	asset->load();
	asset->gl_load();
}

void ExampleApp::draw()
{
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(shader);
	glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 viewMat = glm::lookAt(glm::vec3(4.0, 1.0, -4.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 modelMat = glm::scale(glm::mat4(1.0), glm::vec3(0.01, 0.01, 0.01));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMat));
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniform1i(glGetUniformLocation(shader, "texDiffuse"), 1);

	asset->gl_render();
}

void ExampleApp::event(SDL_Event* event)
{

}