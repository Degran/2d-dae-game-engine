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
			kmo::Box m_hitbox;
		};
	public:
		PhysicsComponent(kmo::PhysicsEngine& engine, kmo::PhysicsInput const& input) noexcept;
		PhysicsComponent(kmo::PhysicsEngine& engine) noexcept
			: PhysicsComponent(engine, kmo::NullPhysicsInput::GetInstance()){}
		void Update(float){}
		void LateUpdate(float deltaTime)
		{
			m_presenceData.m_position += deltaTime * m_input.GetVelocity();
		}
		inline PhysicalPresenceData const& GetPresenceData() const noexcept
		{
			return m_presenceData;
		}
		inline void SetPosition(Vector const& position) noexcept
		{
			m_presenceData.m_position = position;
		}
		inline void SetHitboxSide(float squareSide) noexcept
		{
			m_presenceData.m_hitbox = Box::ConstructBySquareSide(squareSide);
		}
	private:
		PhysicalPresenceData m_presenceData;
		kmo::PhysicsInput const& m_input;
		kmo::PhysicsEngine& m_engine;
	};
}
