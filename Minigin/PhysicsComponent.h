#pragma once
#include "Vector.h"
#include "Box.H"

#include "Component.h"
#include "ControlComponent.h"

namespace kmo
{
	class PhysicsEngine;
	
	class PhysicsComponent final: public Component
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
		void Update(float deltaTime) override;
		inline void LateUpdate(float) override
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
		inline Vector const& GetVelocity() const
		{
			return m_input.GetVelocity();
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
			return GetNextHitbox().IsOverlapping(other.GetNextHitbox());
		}
		inline bool WillStrictlyOverlapNext(PhysicsComponent const & other) const noexcept
		{
			return GetNextHitbox().IsStrictlyOverlapping(other.GetNextHitbox());
		}
		inline void RejectPositionUpdate() noexcept
		{
			m_nextPresenceBuffer = m_currentPresenceBuffer;
		}
		inline Box GetNextHitbox() const noexcept
		{
			return m_properties.m_hitbox.ConstructBoxTranslatedByVector(m_nextPresenceBuffer.m_position);
		}
		inline void RejectPositionUpdate(PhysicalPresenceData buffer) noexcept
		{
			m_nextPresenceBuffer = buffer;
		}
	private:
		PhysicalPresenceData m_currentPresenceBuffer;
		PhysicalPresenceData m_nextPresenceBuffer;
		PhysicalProperties m_properties;
		PhysicsInput const& m_input;
		PhysicsEngine& m_engine;
	};
}
