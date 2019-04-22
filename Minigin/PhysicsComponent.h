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
		PhysicsComponent(kmo::PhysicsInput const& input) noexcept
			: m_input(input){}
		PhysicsComponent() noexcept
			: PhysicsComponent(kmo::NullPhysicsInput::GetInstance()){}
		void Update(float){}
		void LateUpdate(float deltaTime)
		{
			m_presenceData.m_position += deltaTime * m_input.GetVelocity();
		}
		inline PhysicalPresenceData const& GetPresenceData() const
		{
			return m_presenceData;
		}
	private:
		PhysicalPresenceData m_presenceData;
		kmo::PhysicsInput const& m_input;
	};
}
