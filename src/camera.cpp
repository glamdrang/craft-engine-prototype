#include "camera.h"

StandardCamera::StandardCamera()
{
	this->position = glm::vec3(0.0, 1.0, 10.0);

	this->matrix_projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);

	this->buildViewMatrix();
}

void StandardCamera::update()
{
	position.x += _velocity.x * glm::cos(_leftright) + _velocity.z * glm::sin(_leftright);
	position.y += _velocity.y;
	position.z += _velocity.z * glm::cos(_leftright) - _velocity.x * glm::sin(_leftright);

	_updown += _v_updown;
	_leftright += _v_leftright;
	_tilt += _v_tilt;

	this->buildViewMatrix();
}

void StandardCamera::buildViewMatrix()
{
	glm::vec3 const c_up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 look_in = glm::vec3(0.0, 0.0, 0.0);

	float rest = glm::cos(_updown);

	this->vec_look = glm::vec3(rest * glm::sin(_leftright), sin(_updown), rest * glm::cos(_leftright));
	this->vec_left = glm::cross(this->vec_look, c_up);
	this->vec_up = glm::sin(_tilt) * this->vec_left + glm::cos(_tilt) * c_up;

	this->matrix_view = glm::lookAt(this->position, this->position - this->vec_look, this->vec_up);
}

void StandardCamera::handel(SDL_Event const& sdlevent)
{
	float const c_move(0.05f), c_turn(0.002f);

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
			_velocity.x = -c_move; break;
		case SDLK_RIGHT:
		case SDLK_d:
			_velocity.x = c_move; break;
		case SDLK_UP:
		case SDLK_w:
			_velocity.z = -c_move; break;
		case SDLK_DOWN:
		case SDLK_s:
			_velocity.z =  c_move; break;
		case SDLK_q:
			_v_tilt =  c_move; break;
		case SDLK_e:
			_v_tilt = -c_move; break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (sdlevent.key.keysym.sym)
		{
		case SDLK_LEFT:
		case SDLK_a:
			_velocity.x = 0.0; break;
		case SDLK_RIGHT:
		case SDLK_d:
			_velocity.x = 0.0; break;
		case SDLK_UP:
		case SDLK_w:
			_velocity.z = 0.0; break;
		case SDLK_DOWN:
		case SDLK_s:
			_velocity.z = 0.0; break;
		case SDLK_q:
			_v_tilt = 0.0; break;
		case SDLK_e:
			_v_tilt = 0.0; break;
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
