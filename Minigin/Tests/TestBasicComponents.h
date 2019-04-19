#pragma once
#include "../catch.hpp"

#include "../ControlComponent.h"
#include "../PhysicsComponent.h"


SCENARIO("Movement tests")
{
	GIVEN("A velocity")
	{
		kmo::ControlComponent::VelocityData velocityData;
		kmo::PhysicsComponent m_physics(velocityData);
		velocityData.m_velocity = { 5.f, 0.f };
		WHEN("No obstacle")
		{
			kmo::Vector const initialPosition{ m_physics.GetPresenceData().m_position };
			REQUIRE(initialPosition == m_physics.GetPresenceData().m_position);
			const float deltaTime{ 0.03f };
			m_physics.Update(deltaTime);
			m_physics.LateUpdate(deltaTime);
			THEN("Object moves")
			{
				REQUIRE(m_physics.GetPresenceData().m_position != initialPosition);
			}
		}
	}
}
