#pragma once
#include "../common.h"

#include "Vertex.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

class SceneNode
{
public:
	SceneNode* graph_parent;
	std::vector < SceneNode* > graph_children;

	virtual ~SceneNode();
};

class SceneTransformNode : public SceneNode
{
public:
	glm::mat4 transform;
};


class SceneRenderNode : public SceneNode
{
public:
	
};

class Scene
{
public:
	std::vector < SceneNode* > root_nodes;
};
