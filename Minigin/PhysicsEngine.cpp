#include "MiniginPCH.h"
#include "PhysicsEngine.h"

#include "PhysicsComponent.h"

void kmo::PhysicsEngine::CheckCollisions()
{
	for(PhysicsComponent* moving : m_movingComponents)
	{
		CheckCollisionsOfComponent(*moving);
	}
	m_movingComponents.clear();
}

void kmo::PhysicsEngine::CheckCollisionsOfComponent(PhysicsComponent& component)
{
	for(PhysicsComponent* other : m_registeredComponents)
	{
		if(&component == other){ continue; }
		if(component.WillOverlapNext(*other))
		{
			// TODO: Reject position update more intelligently, to allow touching
// 			component.RejectPositionUpdate();
			HandleCollisionNotification(component, *other);
		}
	}
}

void kmo::PhysicsEngine::RejectPositionUpdates(PhysicsComponent& comp1, PhysicsComponent& comp2)
{
	// TODO: if one or the other is a trigger surface, return
	// TODO: if one or the other ignores collisions with objects of the other's type, return
	if(!comp1.WillStrictlyOverlapNext(comp2))
	{
		return;
	}
	comp1.RejectPositionUpdate();
	kmo::Vector collisionVector{ GetCollisionUnitVector(comp1, comp2) };
	kmo::Vector projectedVelocity1{ comp1.GetVelocity().ProjectOntoVector(collisionVector) };
}

void kmo::PhysicsEngine::HandleCollisionNotification(PhysicsComponent const& comp1, PhysicsComponent const& comp2)
{
	if(!comp1.IsOverlapping(comp2))
	{
		// put in the collision queue if not already there
		m_notifier.Notify(CollisionEvent());
		// TODO: if it's a trigger surface (a property) store the collision event in a list 
		// TODO: notify when an object leaves a zone
	}
}

kmo::Vector kmo::PhysicsEngine::GetCollisionUnitVector(PhysicsComponent const&,
	PhysicsComponent const&) const
{
	return { 0.f, 0.f };
}
