#pragma once
#include "common.h"

/* T:
This is an example of a world description header. This is used to "autogenerate" the ECS and other
features of the engine for a given world.
*/

struct CTransform
{
	static constexpr auto c_ecs_componentName = "transform";

	glm::vec3 pos;
	glm::quat rot;
};

struct CRenderable
{
	static constexpr auto c_ecs_componentName = "renderable";
};

struct CPhysics
{
	static constexpr auto c_ecs_componentName = "physics";
	glm::vec3 vel;
};

struct CConfig
{
	static constexpr auto c_ecs_componentName = "config";

};

class ExampleWorld_ECS : public ecs::System <
	ecs::SystemConfig<ecs::SimpleEntityAllocator>,
	ecs::FastComponentManager<CTransform>,
	ecs::FastComponentManager<CPhysics>,
	ecs::MapComponentManager<CConfig>
>
{

};
