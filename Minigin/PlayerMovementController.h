#pragma once
#include <memory>

#include "InputManager.h"
#include "ControlComponent.h"
#include "Vector.h"
#include "Component.h"

namespace kmo
{
	class MovementInputCommand final : public InputCommand
	{
	public:
		MovementInputCommand(PhysicsInputData& velocityData, Vector const& velocityToSet)
			: m_velocity(velocityToSet), m_velocityData(velocityData){}
		inline InputState& ExecuteFromStateToState(InputState& currentState) override
		{
			m_velocityData.m_velocity = m_velocity;
			return currentState;
		}

	private:
		Vector m_velocity;
		kmo::PhysicsInputData& m_velocityData;
	};
	class PlayerMovementController final: public Component
	{
	public:
		inline std::unique_ptr<InputCommand> ConstructMoveRightCommand()
		{
			Vector const velocityToSet{ m_speed, 0.0f };
			return std::make_unique<MovementInputCommand>(m_velocityData, velocityToSet);
		}
		inline std::unique_ptr<InputCommand> ConstructMoveDownCommand()
		{
			Vector const velocityToSet{ 0.0f, m_speed };
			return std::make_unique<MovementInputCommand>(m_velocityData, velocityToSet);
		}
		inline std::unique_ptr<InputCommand> ConstructMoveLeftCommand()
		{
			Vector const velocityToSet{ -m_speed, 0.0f };
			return std::make_unique<MovementInputCommand>(m_velocityData, velocityToSet);
		}
		inline std::unique_ptr<InputCommand> ConstructMoveUpCommand()
		{
			Vector const velocityToSet{ 0.f, -m_speed };
			return std::make_unique<MovementInputCommand>(m_velocityData, velocityToSet);
		}

		inline PhysicsInputData const& GetPhysicsInputData() const
		{
			return m_velocityData;
		}
		inline void Update(float) override{}
		inline void LateUpdate(float) override
		{
			m_velocityData.m_velocity = { 0.f, 0.f };
		}
	private:
		float m_speed{ 50.0f };
		PhysicsInputData m_velocityData;
	};
}
