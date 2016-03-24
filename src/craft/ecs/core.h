#pragma once

/* T:
This is relatively simple, yet powerful, self contained entity component system (ECS). Entity 
component systems are designed to decomplect data and turn large update loops into smaller
executable systems.

An "Entity" is the first key concept, it provides identity. This is often a simple index 
representing a single thing within the ECS. We have two classes of interest an `EntityId` and an
`Entity` which wraps it in a helper class.

A "Component" is the second key concept, it provides data storage. Components are simply any type -
although we do make a requirement that each type may only be used once per system to simplfy the
templates - and a manager to manage them. We provide default implementations for a couple of common
manager designs.

Most ECS designs include some sort of "System" design which executes across the entire ECS. This
one does not provide that feature directly. Instead we provide the tools to build such features
using the ECS in question.

WARNING: This section makes heavy use of templates.
*/

#include "entity.h"
#include "managers.h"
#include "System.h"