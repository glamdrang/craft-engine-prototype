#pragma once

#include "../common.h"

/*
* Definition for a simple opengl asset prep system.
*/

class IAsset
{
public:
	// Loads from file system.
	virtual void load() = 0;

	// Unloads memory (but not from opengl).
	virtual void unload() = 0;

	// Loads asset into opengl.
	virtual void gl_load() = 0;

	// Calls the opengl rendering function.
	// Note: Assumes a shader has been properly bound.
	virtual void gl_render() = 0;

	// Unloads from opengl.
	virtual void gl_unload() = 0;

	// Destroys and deletes.
	virtual void destroy() = 0;

	// Name of asset.
	virtual std::string getName() = 0;
};

IAsset* assetFromFile(std::string filename);
