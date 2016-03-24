#pragma once

#include "craft/common.h"

struct AssetShaderUniforms
{
	GLint color_diffuse;
	GLint texture_diffuse;
};

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
	virtual void gl_render(const AssetShaderUniforms& uniforms) const = 0;

	// Unloads from opengl.
	virtual void gl_unload() = 0;

	// Destroys and deletes.
	virtual void destroy() = 0;

	// Name of asset.
	virtual std::string getName() = 0;
};

/* 
 * Generates an asset from a file using AssImp and SDL_Image.
 */
EXPORTED IAsset* AssetFromFile(std::string filename);
