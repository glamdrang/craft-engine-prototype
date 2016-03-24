#include "craft/common.h"

#include "config.h"

Config::~Config()
{

}

Config* make_config(std::vector<std::string> paths, bool recurse_up_from_cwd)
{
	Config* ret = new Config();
	std::vector<YAML::Node> nodes;

	if (recurse_up_from_cwd)
	{
		std::string path(".");

		for (int i = 0; i < 10; i++)
		{
			path = path::join(path, "..");
			std::string temp_path = path::join(path, "engine.config.yaml");
			if (path::exists(temp_path))
			{
				path = temp_path;
				paths.insert(paths.begin(), path);
				break;
			}
		}
	}

	if (paths.size() == 0)
		return ret;

	ret->rootFile_path = paths[0];
	ret->rootFile_rootNode = YAML::LoadFile(paths[0]);

	return ret;
}