#pragma once
#include "../common.h"

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
