#include "common.h"

namespace engine {
namespace input {

	struct ValueSignal
	{
	};

	struct ValueToggle
	{
		bool state;
	};

	struct ValueForce
	{
		float distance;
	};

	struct ValueForceDual
	{
		float distance_x;
		float distance_y;
	};

	struct ValueAxis
	{
		float position;
	};

	struct ValueAxisDual
	{
		float position_x;
		float position_y;
	};

	struct ValueAxisTriple
	{
		float position_x;
		float position_y;
		float position_z;
	};

}
}