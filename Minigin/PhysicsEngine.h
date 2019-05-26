#pragma once
#include <vector>
#include "Observer.h"
#include "Vector.h"

namespace kmo
{
	class PhysicsComponent;
	class CollisionEvent final{};

	struct ObjectSnapshot final
	{
		Vector m_position;
		char m_typeCode;
	};

	class PhysicsEngine final
	{
	public:
		inline Notifier<kmo::CollisionEvent>& GetNotifier() noexcept
		{
			return m_notifier;
		}
		inline void RegisterComponent(PhysicsComponent& component)
		{
			m_registeredComponents.push_back(&component);
		}
		inline void UnRegisterComponent(PhysicsComponent& component)
		{
			auto ptr{ std::find(m_registeredComponents.begin(), m_registeredComponents.end(), &component) };
			if(ptr == m_registeredComponents.end())
			{
				return;
			}
			m_registeredComponents.erase(ptr);

		}
		inline void RequestPositionUpdate(PhysicsComponent& component)
		{
			m_movingComponents.push_back(&component);
		}

		inline void Update(float)
		{
			CheckCollisions();
			ClearSnapshotCache();
		}
		std::shared_ptr<std::vector<ObjectSnapshot> > GetWorldSnapshot();
		inline void ClearSnapshotCache()
		{
			m_snapshotCache = std::shared_ptr<std::vector<ObjectSnapshot> >(nullptr);
		}
	private:
		void CheckCollisions();
		void CheckCollisionsOfComponent(PhysicsComponent& component);
		void RejectPositionUpdates(PhysicsComponent& comp1, PhysicsComponent& comp2);
		void HandleCollisionNotification(PhysicsComponent const& comp1, PhysicsComponent const& comp2);
		kmo::Vector GetCollisionUnitVector(PhysicsComponent const& comp1, PhysicsComponent const& comp2) const;
		bool IsHorizontalCollision(PhysicsComponent const & comp1, PhysicsComponent const & comp2) const;
		bool IsVerticalCollision(PhysicsComponent const & comp1, PhysicsComponent const & comp2) const;
	private:
		std::vector<PhysicsComponent*> m_registeredComponents;
		std::vector<PhysicsComponent*> m_movingComponents;
		std::vector<PhysicsComponent*> m_componentsToRemove;
		Notifier<kmo::CollisionEvent> m_notifier;
		std::shared_ptr<std::vector<ObjectSnapshot> > m_snapshotCache;
	};
}
