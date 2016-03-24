#pragma once

#include "scene.h"

template <typename TScene>
class IGraphicsLibrary<TScene> abstract
{
public:
	virtual ~IGraphicsLibrary() = 0;

	virtual void SetScene(TScene scene) = 0;
	virtual void Render() = 0;
};

template <typename TScene>
class OpenGL3<Scene> : IGraphicsLibrary < Scene >
{

};