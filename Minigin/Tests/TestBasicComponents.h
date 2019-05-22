#pragma once
#include "../catch.hpp"
#include <memory>

#include "../ControlComponent.h"
#include "../PhysicsComponent.h"
#include "../Observer.h"
#include "../PhysicsEngine.h"


template<class EventType>
class EventChecker final : public kmo::Observer<EventType>
{
public:
	inline void Observe(EventType const& event) override
	{
		m_pLastEvent = std::make_unique<EventType>(event);
	}
	inline bool HasEventFired() const noexcept
	{
		return m_pLastEvent.get() != nullptr;
	}
	inline EventType const* GetLastEvent() const noexcept
	{
		return m_pLastEvent.get();
	}
	inline void Reset() noexcept
	{
		m_pLastEvent = nullptr;
	}
public:
	std::unique_ptr<EventType> m_pLastEvent{nullptr};
};


SCENARIO("Movement tests")
{
	GIVEN("An object with a velocity")
	{
		kmo::PhysicsEngine engine;
		kmo::PhysicsInputData velocityData;
		kmo::PhysicsComponent m_physics(engine, velocityData);
		velocityData.m_velocity = { 1.f, 0.f };
		const float deltaTime{ 0.03f };
		EventChecker<kmo::CollisionEvent> checker;
		engine.GetNotifier().Attach(checker);
		WHEN("No obstacle")
		{
			kmo::Vector const initialPosition{ m_physics.GetPresenceData().m_position };
			REQUIRE(initialPosition == m_physics.GetPresenceData().m_position);
			m_physics.Update(deltaTime);
			m_physics.LateUpdate(deltaTime);
			THEN("Object moves")
			{
				REQUIRE(m_physics.GetPresenceData().m_position != initialPosition);
			}
			THEN("No collision")
			{
				REQUIRE(!checker.HasEventFired());
			}
		}
		WHEN("An obstacle")
		{
			kmo::PhysicsComponent m_obstacle(engine);
			float constexpr finalDistance{ 0.1f };
			float constexpr squareSide{ finalDistance };
			m_physics.SetPosition({ -0.01f, 0.f });
			m_obstacle.SetPosition({ finalDistance , 0.f });
			m_physics.SetHitboxSide(squareSide);
			m_obstacle.SetHitboxSide(squareSide);
			m_physics.Update(deltaTime);
			m_obstacle.Update(deltaTime);
			engine.Update(deltaTime);
			m_physics.LateUpdate(deltaTime);
			m_obstacle.LateUpdate(deltaTime);
			THEN("Collision")
			{
				REQUIRE(checker.HasEventFired());
			}
			WHEN("Another tick with velocity")
			{
				checker.Reset();
				kmo::Vector const position{ m_physics.GetPosition() };
				m_physics.Update(deltaTime);
				m_obstacle.Update(deltaTime);
				engine.Update(deltaTime);
				m_physics.LateUpdate(deltaTime);
				m_obstacle.LateUpdate(deltaTime);
				THEN("No collision")
				{
					REQUIRE(!checker.HasEventFired());
				}
				THEN("Doesn't move")
				{
					REQUIRE(m_physics.GetPosition() == position);
				}
			}
		}
	}
}
