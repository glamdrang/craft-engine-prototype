#include "craft/common.h"
#include "assets.h"

Assets::Assets()
{

}

void Assets::AddHive(std::string hive)
{
	_hives_paths.push_back(hive);
}

std::string Assets::AssetPathToSystemPath(std::string asset_path)
{
	for (auto hive_path : _hives_paths)
	{
		std::string try_path = path::join(hive_path, asset_path);
		if (path::exists(try_path))
			return try_path;
	}
	return "";
}