#pragma once
#include "common.h"
#include "Engine.h"

/******************************************************************************
** User Game
******************************************************************************/

/* T:
Describes the interface for the games used by this teaching engine.
*/
class IGame : public AbstractTypeTag
{
public:
	virtual std::vector<WindowDescription>& windows() = 0;

	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void event(SDL_Event& event) = 0;
};