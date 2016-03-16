#pragma once
#include "../common.h"
#include "../gfx/camera.h"
#include "../input/IInputHandler.h"

class StandardCamera : public ICamera, public IInputHandler
{
	float _updown, _leftright, _tilt;

	bool
		_move_left, _move_right,
		_move_fwd, _move_back,
		_move_up, _move_down,
		_move_tilt_left, _move_tilt_right;

	bool _locked;

	void buildViewMatrix();
public:
	EXPORTED StandardCamera();

	// TODO(Mason): use scene system
	EXPORTED void update();

	virtual void handle(SDL_Event const& sdlevent);
};
