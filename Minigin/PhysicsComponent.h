#pragma once
#include "Vector.h"
#include "Box.H"

#include "ControlComponent.h"

namespace kmo
{
	class PhysicsEngine;
	
	class PhysicsComponent final
	{
	public:
		struct PhysicalPresenceData
		{
			kmo::Vector m_position;
		};
		struct PhysicalProperties
		{
			kmo::Box m_hitbox;
		};
	public:
		PhysicsComponent(kmo::PhysicsEngine& engine, kmo::PhysicsInput const& input) noexcept;
		PhysicsComponent(kmo::PhysicsEngine& engine) noexcept
			: PhysicsComponent(engine, kmo::NullPhysicsInput::GetInstance()){}
		void Update(float deltaTime)
		{
			m_backBuffer.m_position += deltaTime * m_input.GetVelocity();
		}
		void LateUpdate(float)
		{
			m_frontBuffer = m_backBuffer;
		}
		inline PhysicalPresenceData const& GetPresenceData() const noexcept
		{
			return m_frontBuffer;
		}
		inline void SetPosition(Vector const& position) noexcept
		{
			m_frontBuffer.m_position = position;
		}
		inline void SetHitboxSide(float squareSide) noexcept
		{
			m_properties.m_hitbox = Box::ConstructBySquareSide(squareSide);
		}
	private:
		PhysicalPresenceData m_frontBuffer;
		PhysicalPresenceData m_backBuffer;
		PhysicalProperties m_properties;
		kmo::PhysicsInput const& m_input;
		kmo::PhysicsEngine& m_engine;
	};
}
