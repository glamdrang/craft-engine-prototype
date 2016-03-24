#pragma once
#include "craft/common.h"

class Config
{
private:
	friend Config* make_config(std::vector<std::string> paths, bool recurse_up_from_cwd);
public:
	std::string rootFile_path;
	YAML::Node rootFile_rootNode;

	~Config();
};

Config* make_config(std::vector<std::string> paths, bool recurse_up_from_cwd = true);
