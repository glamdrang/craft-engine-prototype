#include "StandardCamera.h"

float const c_move(0.05f), c_turn(0.002f);

StandardCamera::StandardCamera()
{
	this->position = glm::vec3(0.0, 1.0, 10.0);

	this->matrix_projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);

	this->buildViewMatrix();
}

void StandardCamera::update()
{
	glm::vec3 velocity;
	float v_tilt = 0.0;

	if (_move_left) velocity.x += -c_move;
	if (_move_right) velocity.x += c_move;
	if (_move_up) velocity.y += c_move;
	if (_move_down) velocity.y += -c_move;
	if (_move_fwd) velocity.z += -c_move;
	if (_move_back) velocity.z += c_move;
	if (_move_tilt_left) v_tilt += c_move;
	if (_move_tilt_right) v_tilt += -c_move;


	position.x += velocity.x * glm::cos(_leftright) + velocity.z * glm::sin(_leftright);
	position.y += velocity.y;
	position.z += velocity.z * glm::cos(_leftright) - velocity.x * glm::sin(_leftright);

	_tilt += v_tilt;

	this->buildViewMatrix();
}

void StandardCamera::buildViewMatrix()
{
	glm::vec3 const c_up = glm::vec3(0.0, 1.0, 0.0);

	float rest = glm::cos(_updown);

	this->vec_look = glm::vec3(rest * glm::sin(_leftright), sin(_updown), rest * glm::cos(_leftright));
	this->vec_left = glm::cross(this->vec_look, c_up);
	this->vec_up = glm::sin(_tilt) * this->vec_left + glm::cos(_tilt) * c_up;

	this->matrix_view = glm::lookAt(this->position, this->position - this->vec_look, this->vec_up);
}

void StandardCamera::handel(SDL_Event const& sdlevent)
{

	switch (sdlevent.type)
	{
	case SDL_KEYDOWN:
		switch (sdlevent.key.keysym.sym)
		{
		case SDLK_LALT:
			_locked = !_locked;
			SDL_SetRelativeMouseMode(_locked ? SDL_TRUE : SDL_FALSE);
			break;
		case SDLK_LEFT:
		case SDLK_a:
			_move_left = true; break;
		case SDLK_RIGHT:
		case SDLK_d:
			_move_right = true; break;
		case SDLK_UP:
		case SDLK_w:
			_move_fwd = true; break;
		case SDLK_DOWN:
		case SDLK_s:
			_move_back = true; break;
		case SDLK_r:
			_move_up = true; break;
		case SDLK_f:
			_move_down = true; break;
		case SDLK_q:
			_move_tilt_left = true; break;
		case SDLK_e:
			_move_tilt_right = true; break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (sdlevent.key.keysym.sym)
		{
		case SDLK_LEFT:
		case SDLK_a:
			_move_left = false; break;
		case SDLK_RIGHT:
		case SDLK_d:
			_move_right = false; break;
		case SDLK_UP:
		case SDLK_w:
			_move_fwd = false; break;
		case SDLK_DOWN:
		case SDLK_s:
			_move_back = false; break;
		case SDLK_r:
			_move_up = false; break;
		case SDLK_f:
			_move_down = false; break;
		case SDLK_q:
			_move_tilt_left = false; break;
		case SDLK_e:
			_move_tilt_right = false; break;
		default:
			break;
		}
		break;
	case SDL_MOUSEMOTION:
		if (_locked)
		{
			_leftright += -sdlevent.motion.xrel * c_turn;
			_updown += sdlevent.motion.yrel * c_turn;
		}
		break;
	default:
		break;
	}
}
