#include "craft/common.h"

class Assets
{
	std::vector<std::string> _hives_paths;

public:
	Assets();

	EXPORTED void AddHive(std::string hive);

	EXPORTED std::string AssetPathToSystemPath(std::string);
};
