#include "MiniginPCH.h"
#include "PhysicsComponent.h"
#include "PhysicsEngine.h"
#include "GameObject.h"

kmo::PhysicsComponent::PhysicsComponent(kmo::PhysicsEngine& engine, kmo::PhysicsInput const& input) noexcept
	: m_input(input)
	, m_engine(engine)
	, m_parent(std::ref(kmo::GameObject::GetNullGameObject()))
{
	m_engine.RegisterComponent(*this);
}

void kmo::PhysicsComponent::Update(float deltaTime)
{
	if (m_input.GetVelocity().IsZero())
	{
		return;
	}
	// 	m_nextPresenceBuffer.m_position += deltaTime * GetVelocity();
	Vector const movementStep{ deltaTime * GetVelocity().x, deltaTime * GetVelocity().y };
	m_nextPresenceBuffer.m_position += movementStep;
	m_engine.RequestPositionUpdate(*this);
}