#include "MiniginPCH.h"
#include "PhysicsEngine.h"

#include "PhysicsComponent.h"
#include "Box.h"

std::shared_ptr<std::vector<kmo::ObjectSnapshot> const> kmo::PhysicsEngine::GetWorldSnapshot()
{
	if(m_snapshotCache.get() == nullptr)
	{
		auto temp{ std::make_shared<std::vector<kmo::ObjectSnapshot> >() };
		for(PhysicsComponent* component : m_registeredComponents)
		{
			ObjectSnapshot snap;
			snap.m_position = component->GetPosition();
			snap.m_typeCode = component->m_objectTypeCode;
			temp->push_back(snap);
		}
		m_snapshotCache = temp;
	}
	return m_snapshotCache;
}

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
			RejectPositionUpdates(component, *other);
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
// 	comp1.RejectPositionUpdate();
// 	const kmo::Vector collisionVector{ GetCollisionUnitVector(comp1, comp2) };
// 	kmo::Vector projectedVelocity1{ comp1.GetVelocity().ProjectOntoVector(collisionVector) };
	Vector comp1CollisionVector, comp2CollisionVector;
	if(IsHorizontalCollision(comp1, comp2))
	{
		comp1CollisionVector = kmo::Vector::ConstructSignedXUnitVector(comp2.GetPosition().x - comp1.GetPosition().x);
		comp2CollisionVector = kmo::Vector::ConstructSignedXUnitVector(comp1.GetPosition().x - comp2.GetPosition().x);
	}else if(IsVerticalCollision(comp1, comp2))
	{
		comp1CollisionVector = kmo::Vector::ConstructSignedXUnitVector(comp2.GetPosition().y - comp1.GetPosition().y);
		comp2CollisionVector = kmo::Vector::ConstructSignedXUnitVector(comp1.GetPosition().y - comp2.GetPosition().y);
	}
	Vector const comp1CurrentEffectiveVelocity{ comp1.m_nextPresenceBuffer.m_position - comp1.GetPosition() };
	Vector const comp2CurrentEffectiveVelocity{ comp2.m_nextPresenceBuffer.m_position - comp2.GetPosition() };
	float const comp1MovementLength{ comp1CurrentEffectiveVelocity.PositiveProjectionLengthAlongVector(comp1CollisionVector) };
	float const comp2MovementLength{ comp2CurrentEffectiveVelocity.PositiveProjectionLengthAlongVector(comp2CollisionVector) };
	float const totalLength{ comp1MovementLength + comp2MovementLength };
	float const comp1MovementWeight{ comp1MovementLength / totalLength };
	float const comp2MovementWeight{ comp2MovementLength / totalLength };
	// TODO: if the weight is 0, use the next buffer for the distance
	// TODO: i.e. one of the objects was moving away
	PhysicsComponent::PhysicalPresenceData comp1OriginBuffer{ comp1.m_currentPresenceBuffer };
	PhysicsComponent::PhysicalPresenceData comp2OriginBuffer{ comp2.m_currentPresenceBuffer };
	Vector const displacementVector{ comp2OriginBuffer.m_position - comp1OriginBuffer.m_position};
	Vector const collisionDistanceVector{ displacementVector.ProjectOntoVector(comp1CollisionVector) };
	float const comp1HalfHitbox{ comp1.GetCurrentHitBox().GetDimensionsVector().ProjectOntoVector(comp1CollisionVector).GetL1Magnitude() / 2.0f };
	float const comp2HalfHitbox{ comp2.GetCurrentHitBox().GetDimensionsVector().ProjectOntoVector(comp2CollisionVector).GetL1Magnitude() / 2.0f };
	float const perfectFloatDistance{ collisionDistanceVector.GetMagnitude() - comp1HalfHitbox - comp2HalfHitbox };
	float const distanceToCover{perfectFloatDistance + Interval::STRICT_OVERLAP_MARGIN / 2.0f};
	if(0.0f < comp1MovementWeight)
	{
		Vector const parallelVelocityComponent{ comp1CurrentEffectiveVelocity.ProjectOntoVector(comp1CollisionVector) };
		Vector const orthogonalVelocityComponent{ comp1CurrentEffectiveVelocity - parallelVelocityComponent };
		Vector const scaledParallel{ comp1MovementWeight * distanceToCover * comp1CollisionVector};
		Vector const scaledTotal{ scaledParallel + orthogonalVelocityComponent };
		PhysicsComponent::PhysicalPresenceData comp1NextBuffer{ comp1OriginBuffer };
		comp1NextBuffer.m_position = comp1OriginBuffer.m_position + scaledTotal;
		comp1.RejectPositionUpdate(comp1NextBuffer);
	}
	if(0.0f < comp2MovementWeight)
	{
		Vector const parallelVelocityComponent{ comp2CurrentEffectiveVelocity.ProjectOntoVector(comp2CollisionVector) };
		Vector const orthogonalVelocityComponent{ comp2CurrentEffectiveVelocity - parallelVelocityComponent };
		Vector const scaledParallel{ comp1MovementWeight * distanceToCover * comp2CollisionVector};
		Vector const scaledTotal{ scaledParallel + orthogonalVelocityComponent };
		PhysicsComponent::PhysicalPresenceData comp2NextBuffer{ comp2OriginBuffer };
		comp2NextBuffer.m_position = comp2OriginBuffer.m_position + scaledTotal;
		comp2.RejectPositionUpdate(comp2NextBuffer);
	}

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

kmo::Vector kmo::PhysicsEngine::GetCollisionUnitVector(PhysicsComponent const& comp1,
	PhysicsComponent const& comp2) const
{
	if(IsHorizontalCollision(comp1, comp2))
	{
		return kmo::Vector::ConstructSignedXUnitVector(comp2.GetPosition().x - comp1.GetPosition().x);
	}else if(IsVerticalCollision(comp1, comp2))
	{
		return kmo::Vector::ConstructSignedYUnitVector(comp2.GetPosition().y - comp1.GetPosition().y);
	}
	// Ignore the case for now where they are perfectly diagonally alined
	return { 0.f, 0.f };
}

bool kmo::PhysicsEngine::IsHorizontalCollision(PhysicsComponent const& comp1, PhysicsComponent const& comp2) const
{
	return comp1.GetCurrentHitBox().ConstructYInterval().IsOverlappingOrTouches(comp2.GetCurrentHitBox().ConstructYInterval());
}

bool kmo::PhysicsEngine::IsVerticalCollision(PhysicsComponent const& comp1, PhysicsComponent const& comp2) const
{
	return comp1.GetCurrentHitBox().ConstructXInterval().IsOverlappingOrTouches(comp2.GetCurrentHitBox().ConstructXInterval());
}
