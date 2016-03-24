#pragma once
#include "craft/common.h"
#include "Engine.h"

/******************************************************************************
** User Game
******************************************************************************/

/* T:
Describes the interface for the games used by this teaching engine.
*/
class IGame
{
public:
	virtual std::vector<WindowDescription>& windows() = 0;

	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void event(SDL_Event& event) = 0;
};
