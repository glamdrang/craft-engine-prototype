#pragma once

#include "common.h"

/* T:
This describes the engine subsystems.
*/

#include "input/IInputHandler.h"
#include "input/console.h"

#include "gfx/camera.h"
#include "gfx/shader.h"
#include "gfx/asset.h"

#include "features/handlers.h"
#include "features/StandardCamera.h"

/* T:
This describes the core engine singleton, tying togeather all of the above.
*/

#include "Engine.h"
