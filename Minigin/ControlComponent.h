#pragma once
#include "Vector.h"

namespace kmo
{
	class ControlComponent final
	{
	public:
		struct VelocityData
		{
			kmo::Vector m_velocity;
		};
	public:
		VelocityData m_velocityData;
	};
}
