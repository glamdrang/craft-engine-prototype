#pragma once
#include "common.h"

namespace path
{

/******************************************************************************
** Implementations
******************************************************************************/

	std::string join(std::string const& first, std::string const& second);
	std::string dir(std::string const& path);
	bool exists(std::string const& path);

/******************************************************************************
** Inline Helpers
******************************************************************************/

	inline std::string join(std::string a0, std::string a1, std::string last)
	{
		return join(join(a0, a1), last);
	}
	inline std::string join(std::string a0, std::string a1, std::string a2, std::string last)
	{
		return join(join(a0, a1, a2), last);
	}
	inline std::string join(std::string a0, std::string a1, std::string a2, std::string a3, std::string last)
	{
		return join(join(a0, a1, a2, a3), last);
	}

}