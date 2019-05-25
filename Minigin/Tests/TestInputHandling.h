#pragma once
#include "../catch.hpp"
#include <vector>

#include "../InputManager.h"
#include "../PlayerMovementController.h"

class MockInputSource final: public kmo::InputSource{
public:
	inline std::vector<kmo::InputEvent> GetInputEvents() override
	{
		return m_events;
	}
	inline void ListenForEvent(kmo::InputEvent const&) override{}
	inline void AddEvent(kmo::InputEvent const& event)
	{
		m_events.push_back(event);
	}
	inline void ClearEvents()
	{
		m_events.clear();
	}
private:
	std::vector<kmo::InputEvent> m_events;
};

SCENARIO("Input handling events")
{
	GIVEN("An InputManager and player")
	{
		std::unique_ptr<MockInputSource> tempSource{ std::make_unique<MockInputSource>() };
		MockInputSource& sourceRef = *tempSource;
		kmo::PlayerMovementController controller;
		const kmo::InputEvent mockLeft;
		std::unique_ptr<kmo::StandardInputState> tempDefaultState{ std::make_unique<kmo::StandardInputState>() };
		tempDefaultState->AssignCommandToInput(controller.ConstructMoveLeftCommand(), mockLeft);
		kmo::InputManager manager(std::move(tempDefaultState));
		manager.SetInputSource(std::move(tempSource));
		const float deltaTime{ 0.03f };
		WHEN("Nothing")
		{
			manager.ProcessInput();
			controller.Update(deltaTime);
			THEN("No velocity")
			{
				REQUIRE(controller.GetPhysicsInputData().GetVelocity().IsZero());
			}
		}
		WHEN("Left arrow")
		{
			sourceRef.AddEvent(mockLeft);
			manager.ProcessInput();
			controller.Update(deltaTime);
			THEN("Velocity")
			{
				REQUIRE(!controller.GetPhysicsInputData().GetVelocity().IsZero());
			}
		}
	}
}
