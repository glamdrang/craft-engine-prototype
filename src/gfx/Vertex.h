#pragma once
#include "../common.h"

/******************************************************************************
** Vertex
******************************************************************************/

// This is a recursive vertex definition.
// Base case.
template <typename... Ts> class Vertex { };

// This is a templated vertex definition.
// General case.
template <typename T, typename... Ts>
class Vertex<T, Ts...> : Vertex<Ts...>
{
public:
	// Recursive constructors
	Vertex(T t, Ts... ts) : Vertex<Ts...>(ts...), elem(t) {}
	Vertex() : Vertex<Ts...>(), elem() {}

	// Element for this recursive definition
	T elem;

	// Gets a value from the vertex by index
	// Base case
	template <size_t k>
	typename std::enable_if<k == 0,
		typename _T_elem_index<0, T, Ts...>::type&
	>::type
	get() {
		return this->elem;
	}

	// Gets a value from the vertex by index
	// General case
	template <size_t k>
	typename std::enable_if<k != 0 /*&& k <= T_detail<Vertex<T, Ts...>>::elem_count*/,
		typename _T_elem_index<k, T, Ts...>::type&
	>::type
	get() {
		Vertex<Ts...>& base = *this;
		return Vertex<Ts...>::get<k - 1>();
	}
};

/******************************************************************************
** Vertex Utility Declarations and Bases
******************************************************************************/

// Vertex virtual class
class VertexFormatBase abstract
{
public:
	virtual ~VertexFormatBase();

	virtual size_t ElementCount() const = 0;
	virtual std::string const& ElementName(size_t index) const = 0;
	virtual size_t ElementSize(size_t index) const = 0;
};

// Vertex format forward declaration
template <class TVertex>
class VertexFormat;

// Vertex virtual class
class VertexBufferBase abstract
{
public:
	virtual ~VertexBufferBase();

	virtual size_t ElementCount() const = 0;
	virtual std::string const& ElementName(size_t index) const = 0;
	virtual size_t ElementSize(size_t index) const = 0;
};

// Vertex buffer forward declaration
template <class TVertex, typename TIds>
class VertexBuffer;

/******************************************************************************
** VertexFormat
******************************************************************************/

// Vertex format helper class for binding names to vertex positions
template <typename... VTs>
class VertexFormat < Vertex<VTs...> > : public VertexFormatBase
{
public:
	typedef typename Vertex<VTs...> TVertex;

	const static size_t elem_count = T_detail<TVertex>::elem_count;

	// Array of names (constant size)
	std::string elem_names[elem_count];

	inline VertexFormat()
	{
		for (size_t i = 0; i <= elem_count; i++)
		{
			elem_names[i] = fmt::format("v{0}", i);
		}
	}

	inline size_t indexOf(std::string const& element) const
	{
		for (size_t i = 0; i <= elem_count; i++)
		{
			if (element == elem_names[i])
			{
				return i;
			}
		}
	}

	inline std::string const& nameOf(size_t index) const
	{
		return elem_names[i];
	}

public:
	inline virtual ~VertexFormat() override
	{
		// empty
	}

	virtual size_t ElementCount() const override
	{
		return elem_count;
	}

	virtual std::string const& ElementName(size_t index) const override
	{
		return elem_names[index];
	}

	virtual size_t ElementSize(size_t index) const override
	{
		return sizeof(_T_elem_index<index, TVertex>::type);
	}
};

/******************************************************************************
** VertexBuffer
******************************************************************************/

// Vertex buffer helper class for containing vertex data
template <typename TIds, typename... VTs>
class VertexBuffer < Vertex<VTs...>, TIds> : public VertexBufferBase
{
public:
	typedef typename Vertex<VTs...> TVertex;

	size_t length;
	VertexFormat<TVertex>* format;
	TVertex* buffer;
	
	TIds id;

	inline VertexBuffer(size_t length, VertexFormat<TVertex>* format) : length(length), format(format), buffer(nullptr) { }
	inline ~VertexBuffer()
	{
		deallocate();
	}

	inline void allocate()
	{
		buffer = new TVertex[length];
	}

	inline void deallocate()
	{
		if (buffer != nullptr)
		{
			delete[] buffer;
		}
	}

	inline void resize(size_t length)
	{
		TVertex* old_buffer = this->buffer;
		size_t old_size = this->size;

		this->length = length;
		this->buffer = nullptr;
		allocate();

		std::copy(old_buffer, old_buffer + length, this->buffer);

		delete[] old_buffer;
	}

	inline TVertex& operator[](std::size_t index) { return this->buffer[index]; }
	inline TVertex const& operator[](std::size_t index) const { return this->buffer[index]; }
};
