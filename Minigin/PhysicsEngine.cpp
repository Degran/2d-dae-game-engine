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
			component.RejectPositionUpdate();
			HandleCollisionNotification(component, *other);
		}
	}
}

void kmo::PhysicsEngine::HandleCollisionNotification(PhysicsComponent const& comp1, PhysicsComponent const& comp2)
{
	if(!comp1.IsOverlapping(comp2))
	{
		// put in the collision queue if not already there
		m_notifier.Notify(CollisionEvent());
		// TODO: if it's a zone (a property) store the collision event in a list 
		// TODO: notify when an object leaves a zone
	}
}
