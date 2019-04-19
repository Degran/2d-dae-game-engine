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
		inline VelocityData const& GetVelocity() const
		{
			return m_velocityData;
		}
	private:
		VelocityData m_velocityData;
	};
}
