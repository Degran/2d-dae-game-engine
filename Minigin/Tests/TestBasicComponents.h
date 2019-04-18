#pragma once
#include "../catch.hpp"

#include "../ControlComponent.h"
#include "../PhysicsComponent.h"


// struct TestComponents
// {
// 	kmo::ControlComponent m_control;
// 	kmo::PhysicsComponent m_physics;
// };
// 
// inline TestComponents GivenAVelocity()
// {
// 	TestComponents result;
// 	result.m_control.m_velocityData.m_velocity = { 5.f, 0.f };
// 	return result;
// }
// 
// inline void WhenNoObstacle(){}
// 
// inline void ThenObjectMoves()
// {
// 	
// }
// 
// TEST_CASE("Basic move")
// {
// 	// REQUIRE(*pEncrypted != 'a');
// 	TestComponents components{ GivenAVelocity() };
// 	WhenNoObstacle();
// 	ThenObjectMoves();
// }

SCENARIO("Movement tests")
{
	GIVEN("A velocity")
	{
		kmo::ControlComponent m_control;
		kmo::PhysicsComponent m_physics(m_control.m_velocityData);
		m_control.m_velocityData.m_velocity = { 5.f, 0.f };
		WHEN("No obstacle")
		{
			kmo::Vector initialPosition{ m_physics.m_presenceData.m_position };
			REQUIRE(initialPosition == m_physics.m_presenceData.m_position);
			float deltaTime{ 0.03f };
			m_physics.Update(deltaTime);
			m_physics.LateUpdate(deltaTime);
			THEN("Object moves")
			{
				REQUIRE(m_physics.m_presenceData.m_position != initialPosition);
			}
		}
	}
}
