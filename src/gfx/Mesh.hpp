#pragma once
#include "../common.h"

#include "Triangle.hpp"
#include "Vertex.hpp"

/******************************************************************************
** Mesh
******************************************************************************/

template <class TVertex>
class Mesh;

template <typename... VTs>
class Mesh<Vertex<VTs...>>
{
public:
	typedef typename Vertex<VTs...> TVertex;

	std::vector<TVertex> verticies;
	std::vector<t_triangle_index> triangle_strip;
};
