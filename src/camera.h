#pragma once
#include "common.h"
#include "event_input.h"

class ICamera
{
public:
	glm::vec3 position;

	glm::vec3 vec_up;
	glm::vec3 vec_look;
	glm::vec3 vec_left;

	glm::mat4 matrix_projection;
	glm::mat4 matrix_view;
};

class StandardCamera : public ICamera, public IInputHandler
{
	float _updown, _leftright, _tilt;
	float _v_updown, _v_leftright, _v_tilt;
	
	glm::vec3 _velocity;

	bool _locked;

	void buildViewMatrix();
public:
	StandardCamera();

	// TODO(Mason): use scene system
	void update();

	virtual void handel(SDL_Event const& sdlevent);
};