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
		MockInputSource& sourceRef{ *tempSource };
		kmo::PlayerMovementController controller;
		const kmo::InputEvent mockLeft;
		std::unique_ptr<kmo::StandardInputState> tempDefaultState{ std::make_unique<kmo::StandardInputState>() };
		tempDefaultState->AssignCommandToInput(controller.ConstructMoveLeftCommand(), mockLeft);
		kmo::StandardInputState& stateRef{ *tempDefaultState };
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
			WHEN("Next tick nothing")
			{
				controller.LateUpdate(deltaTime);
				sourceRef.ClearEvents();
				manager.ProcessInput();
				controller.Update(deltaTime);
				THEN("No velocity")
				{
					REQUIRE(controller.GetPhysicsInputData().GetVelocity().IsZero());
				}
			}
		}
		GIVEN("Second player")
		{
			kmo::PlayerMovementController controllerPlayer2;
			kmo::InputEvent mockUp2;
			mockUp2.m_deviceId = 1;
			mockUp2.m_keyCode = 0x0001;
			stateRef.AssignCommandToInput(controllerPlayer2.ConstructMoveUpCommand(), mockUp2);
			WHEN("Left arrow one and up arrow two")
			{
				sourceRef.AddEvent(mockLeft);
				sourceRef.AddEvent(mockUp2);
				manager.ProcessInput();
				controller.Update(deltaTime);
				controllerPlayer2.Update(deltaTime);
				THEN("Expected velocities")
				{
					kmo::Vector const expectedPlayer1Velocity{ -50.f, 0.f };
					kmo::Vector const expectedPlayer2Velocity{ 0.f, -50.f };
					REQUIRE(controller.GetPhysicsInputData().GetVelocity() == expectedPlayer1Velocity);
					REQUIRE(controllerPlayer2.GetPhysicsInputData().GetVelocity() == expectedPlayer2Velocity);
				}
			}
			GIVEN("An assigned pause toggle button")
			{
				kmo::InputEvent mockPause;
				mockPause.m_keyCode = 0x0002;
				std::unique_ptr<kmo::StandardInputState> tempPauseState{ std::make_unique<kmo::StandardInputState>() };
				kmo::StandardInputState& pauseRef{ *tempPauseState };
				stateRef.AssignCommandToInput(std::make_unique<kmo::StateTransitionInputCommand>(pauseRef), mockPause);
				pauseRef.AssignCommandToInput(std::make_unique<kmo::StateTransitionInputCommand>(stateRef), mockPause);
				manager.AddInputState(std::move(tempPauseState));
				WHEN("Pause pressed")
				{
					sourceRef.AddEvent(mockPause);
					manager.ProcessInput();
					controller.Update(deltaTime);
					controllerPlayer2.Update(deltaTime);
					controller.LateUpdate(deltaTime);
					controllerPlayer2.LateUpdate(deltaTime);
					WHEN("Then left arrow one pressed")
					{
						sourceRef.ClearEvents();
						sourceRef.AddEvent(mockLeft);
						manager.ProcessInput();
						controller.Update(deltaTime);
						controllerPlayer2.Update(deltaTime);
						THEN("No velocity")
						{
							REQUIRE(controller.GetPhysicsInputData().GetVelocity().IsZero());
						}
					}
				}
			}
		}
	}
}
