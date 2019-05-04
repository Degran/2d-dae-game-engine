#include "MiniginPCH.h"
#include "PhysicsEngine.h"

#include "PhysicsComponent.h"

void kmo::PhysicsEngine::CheckCollisions(float)
{
	for(PhysicsComponent* moving : m_movingComponents)
	{
		for(PhysicsComponent* other : m_registeredComponents)
		{
			if(moving == other)
			{
				continue;
			}
			if(moving->WillOverlapNext(*other))
			{
				// Reject position update
				// TODO
				if(!moving->IsOverlapping(*other))
				{
					// put in the collision queue if not already there
					m_notifier.Notify(CollisionEvent());
				}
			}
		}
	}
	m_movingComponents.clear();
}
