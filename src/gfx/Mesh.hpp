#pragma once
#include "../common.h"


/******************************************************************************
** Vertex
******************************************************************************/

template <typename... Ts> class Vertex { };

template <typename T, typename... Ts>
class Vertex<T, Ts...> : Vertex<Ts...>
{
	Vertex(T t, Ts... ts) : Vertex<Ts...>(ts...), elem(t) {}

	T elem;

	template <size_t k, class... Ts>
		typename std::enable_if<k == 0, 
			typename _T_elem_index<0, Vertex<Ts...>>::type&
		>::type
	get(Vertex<Ts...>& t) {
		return t.elem;
	}

	template <size_t k, class T, class... Ts>
		typename std::enable_if<k != 0 && k <= T_detail<Vertex<Ts...>>::elem_count,
			typename _T_elem_index<k, Vertex<Ts...>>::type&
		>::type
	get(Vertex<T, Ts...>& t) {
		Vertex<Ts...>& base = t;
		return get<k - 1>(base);
	}
};

/******************************************************************************
** Triangle
******************************************************************************/

typedef unsigned int t_triangle_index;

class Triangle
{
public:
	t_triangle_index a, b, c;
};

/******************************************************************************
** Mesh
******************************************************************************/

template <class TVertex>
class Mesh;

template <typename... VTs>
class Mesh<Vertex<VTs...>>
{
public:
	typedef class Vertex<VTs...> TVertex;

	std::vector<TVertex> verticies;
	std::vector<t_triangle_index> triangle_strip;
};
