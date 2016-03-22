#include "common.h"
#include "path.h"

#include <windows.h>
#include <Shlwapi.h>

std::string path::join(std::string const& first, std::string const& second_)
{
	std::string second = second_;
	if (second == "..")
		second = "../";

	char out_buffer[MAX_PATH] = "";
	char* res = PathCombine(out_buffer, first.c_str(), second.c_str());

	if (res == nullptr)
		return std::string("");

	return std::string(out_buffer);
}

std::string path::normalize(std::string const& path)
{
	// TODO actually normalize
	return path;
}

std::string path::dir(std::string const& path)
{
	std::vector<char> buffer(path.begin(), path.end());
	buffer.push_back('\0');
	
	BOOL res = PathRemoveFileSpec(buffer.data());

	if (res != TRUE)
		return std::string("");

	return std::string(buffer.data());
}

bool path::exists(std::string const& path)
{
	// TODO: determine file or dir and check dir existence
	return PathFileExists(path.c_str()) == TRUE;
}

std::vector<std::string> path::list_dirs(std::string const& path)
{
	std::vector<std::string> ret;

	// TODO actually list

	return ret;
}

std::vector<std::string> path::list_files(std::string const& path)
{
	std::vector<std::string> ret;

	// TODO actually list

	return ret;
}