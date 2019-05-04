#include "MiniginPCH.h"
#include "PhysicsComponent.h"
#include "PhysicsEngine.h"

kmo::PhysicsComponent::PhysicsComponent(kmo::PhysicsEngine& engine, kmo::PhysicsInput const& input) noexcept
	: m_input(input), m_engine(engine){
	m_engine.RegisterComponent(*this);
}

void kmo::PhysicsComponent::Update(float deltaTime)
{
	if(m_input.GetVelocity().IsZero())
	{
		return;
	}
	m_nextPresenceBuffer.m_position += deltaTime * m_input.GetVelocity();
	m_engine.RequestPositionUpdate(*this);
}