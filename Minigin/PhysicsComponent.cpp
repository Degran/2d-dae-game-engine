#include "MiniginPCH.h"
#include "PhysicsComponent.h"
#include "PhysicsEngine.h"

kmo::PhysicsComponent::PhysicsComponent(kmo::PhysicsEngine& engine, kmo::PhysicsInput const& input) noexcept
	: m_input(input), m_engine(engine){
	m_engine.RegisterComponent(*this);
}
