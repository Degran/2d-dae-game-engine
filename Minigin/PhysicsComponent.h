#pragma once
#include "Vector.h"

#include "ControlComponent.h"

namespace kmo
{
	class PhysicsComponent final
	{
	public:
		struct PhysicalPresenceData
		{
			kmo::Vector m_position;
		};
	public:
		PhysicsComponent(kmo::ControlComponent::VelocityData const& inputVelocity) noexcept
			: m_inputVelocity(inputVelocity){}
		void Update(float){}
		void LateUpdate(float deltaTime)
		{
			m_presenceData.m_position += deltaTime * m_inputVelocity.m_velocity;
		}
		inline PhysicalPresenceData const& GetPresenceData() const
		{
			return m_presenceData;
		}
	private:
		PhysicalPresenceData m_presenceData;
		kmo::ControlComponent::VelocityData const& m_inputVelocity;
	};
}
