#pragma once
#include "Component.h"
#include "ControlComponent.h"

namespace digdug
{
	class FygarController final : public kmo::Component
	{
	public:
		inline kmo::PhysicsInputData const& GetPhysicsInputData() const
		{
			return m_velocityData;
		}

		void Update(float) override
		{
			
		}
		void LateUpdate(float) override
		{
			
		}
	public:
		std::reference_wrapper<kmo::Vector const>  m_position{ std::ref(kmo::ControlComponent::NullPosition()) };
	private:
		void AttachTo(kmo::GameObject&) override{}
		kmo::PhysicsInputData m_velocityData;
	};
}
