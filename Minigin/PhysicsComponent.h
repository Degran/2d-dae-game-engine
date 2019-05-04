#pragma once
#include "Vector.h"
#include "Box.H"

#include "ControlComponent.h"

namespace kmo
{
	class PhysicsEngine;
	
	class PhysicsComponent final
	{
		friend PhysicsEngine;
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
		void Update(float deltaTime);
		void LateUpdate(float)
		{
			m_currentPresenceBuffer = m_nextPresenceBuffer;
		}
		inline PhysicalPresenceData const& GetPresenceData() const noexcept
		{
			return m_currentPresenceBuffer;
		}
		inline Vector const& GetPosition() const noexcept
		{
			return m_currentPresenceBuffer.m_position;
		}
		inline Box GetCurrentHitBox() const noexcept
		{
			return m_properties.m_hitbox.ConstructBoxTranslatedByVector(GetPosition());
		}
		inline void SetPosition(Vector const& position) noexcept
		{
			m_currentPresenceBuffer.m_position = position;
			m_nextPresenceBuffer.m_position = position;
		}
		inline void SetHitboxSide(float squareSide) noexcept
		{
			m_properties.m_hitbox = Box::ConstructBySquareSide(squareSide);
		}
		inline bool IsOverlapping(PhysicsComponent const& other) const noexcept
		{
			return GetCurrentHitBox().IsOverlapping(other.GetCurrentHitBox());
		}
	private:
		inline bool WillOverlapNext(PhysicsComponent const & other) const noexcept
		{
			Box const thisNextHitbox{ m_properties.m_hitbox.ConstructBoxTranslatedByVector(m_nextPresenceBuffer.m_position) };
			Box const otherNextHitbox{ other.m_properties.m_hitbox.ConstructBoxTranslatedByVector(other.m_nextPresenceBuffer.m_position) };
			return thisNextHitbox.IsOverlapping(otherNextHitbox);
		}
		inline void RejectPositionUpdate() noexcept
		{
			m_nextPresenceBuffer = m_currentPresenceBuffer;
		}
		inline void RejectPositionUpdate(PhysicalPresenceData buffer) noexcept
		{
			m_nextPresenceBuffer = buffer;
		}
		inline bool IsCollision(PhysicsComponent const& other) const noexcept
		{
			bool const isNotOverlapping{ !IsOverlapping(other) };
			bool const willOverlap{ WillOverlapNext(other) };
			return isNotOverlapping && willOverlap;
		}
	private:
		PhysicalPresenceData m_currentPresenceBuffer;
		PhysicalPresenceData m_nextPresenceBuffer;
		PhysicalProperties m_properties;
		kmo::PhysicsInput const& m_input;
		kmo::PhysicsEngine& m_engine;
	};
}
