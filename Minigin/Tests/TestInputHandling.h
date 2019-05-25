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
		kmo::InputManager manager;
		std::unique_ptr<kmo::InputSource> source{ std::make_unique<MockInputSource>() };
		manager.SetInputSource(std::move(source));
// 		kmo::PlayerMovementController controller;
// 		const kmo::InputEvent mockLeft;
// 		kmo::StandardInputState defaultState;
// 		defaultState.AssignCommandToInput(std::make_unique<kmo::InputCommand>(), mockLeft);
		WHEN("Nothing")
		{
			manager.ProcessInput();
			THEN("No velocity")
			{
				REQUIRE(true);
			}
		}
	}
}
