#pragma once
#include <future>
#include <memory>
#include <vector>

#include "Component.h"
#include "ControlComponent.h"
#include "PhysicsEngine.h"

namespace digdug
{
	class FygarController final : public kmo::Component
	{
	private:
		struct DecisionResults
		{
			kmo::Vector m_velocity;
			// TODO: add reference to state to transition to
		};
	public:
		FygarController(kmo::PhysicsEngine& engine)
			: m_engine(engine){}
		inline kmo::PhysicsInputData const& GetPhysicsInputData() const
		{
			return m_velocityData;
		}

		inline void Update(float) override
		{
			if(m_nextActions.valid())
			{
				m_velocityData.m_velocity = m_nextActions.get().m_velocity;
			}
			auto snapshot{ m_engine.GetWorldSnapshot() };
			kmo::Vector currentPosition{ m_position.get() };
			m_nextActions = std::async(std::launch::async, &FygarController::PlanNextActionsFromPositionInWorld, this, currentPosition, snapshot);
		}
		inline void LateUpdate(float) override
		{
			
		}
		inline DecisionResults PlanNextActionsFromPositionInWorld(kmo::Vector , std::shared_ptr<std::vector<kmo::ObjectSnapshot> const> )
		{
			return DecisionResults();
		}
	public:
		std::reference_wrapper<kmo::Vector const>  m_position{ std::ref(kmo::ControlComponent::NullPosition()) };
	private:
		void AttachTo(kmo::GameObject&) override{}
	private:
		kmo::PhysicsInputData m_velocityData;
		kmo::PhysicsEngine& m_engine;
		std::future<DecisionResults> m_nextActions;
	};
}
