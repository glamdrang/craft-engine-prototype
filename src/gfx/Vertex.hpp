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
** VertexFormat
******************************************************************************/

template <class TVertex>
class VertexFormat;

template <typename... VTs>
class VertexFormat < Vertex<VTs...> >
{
public:
	typedef typename Vertex<VTs...> TVertex;

	const static size_t elem_count = T_detail<TVertex>::elem_count;

	std::string elem_names[elem_count];

	inline VertexFormat()
	{
		for (size_t i = 0; i <= elem_count; i++)
		{
			elem_names[i] = fmt::format("v{0}", i);
		}
	}

	inline size_t indexOf(std::string& element)
	{
		for (size_t i = 0; i <= elem_count; i++)
		{
			if (element == elem_names[i])
			{
				return i;
			}
		}
	}
};

/******************************************************************************
** VertexBuffer
******************************************************************************/

template <class TVertex>
class VertexBuffer;

template <typename... VTs>
class VertexBuffer < Vertex<VTs...> >
{
public:
	typedef typename Vertex<VTs...> TVertex;

	size_t size;
	VertexFormat* format;
	TVertex* buffer;
	
	t_glid _gl_id;

	inline VertexBuffer(size_t size, VertexFormat* format) : size(size), format(format), buffer(nullptr) { }
	inline ~VertexBuffer()
	{
		deallocate();
	}

	inline allocate()
	{
		buffer = new TVertex[size];
	}

	inline deallocate()
	{
		if (buffer != nullptr)
		{
			delete[] buffer;
		}
	}

	inline resize(size_t size)
	{
		TVertex* old_buffer = this->buffer;
		size_t old_size = this->size;

		this->size = size;
		this->buffer = nullptr;
		allocate();

		std::copy(old_buffer, old_buffer + size, this->buffer);

		delete[] old_buffer;
	}

	inline TVertex& operator[](std::size_t index) { return this->buffer[index]; }
	inline TVertex const& operator[](std::size_t index) const { return this->buffer[index]; }
};
