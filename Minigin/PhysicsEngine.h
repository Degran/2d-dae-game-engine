#pragma once
#include <vector>
#include "Observer.h"
#include "Vector.h"

namespace kmo
{
	class PhysicsComponent;
	class CollisionEvent final{};

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
// 			m_componentsToRemove.push_back(&component);
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
// 			RemoveComponents();
		}
	private:
		void CheckCollisions();
// 		inline void RemoveComponents()
// 		{
// 			for(PhysicsComponent* compPtr : m_componentsToRemove)
// 			{
// 				auto ptr{ std::find(m_registeredComponents.begin(), m_registeredComponents.end(), compPtr) };
// 				if(ptr == m_registeredComponents.end())
// 				{
// 					continue;
// 				}
// 				m_registeredComponents.erase(ptr);
// 			}
// 		}
		void CheckCollisionsOfComponent(PhysicsComponent& component);
		void RejectPositionUpdates(PhysicsComponent& comp1, PhysicsComponent& comp2);
		void HandleCollisionNotification(PhysicsComponent const& comp1, PhysicsComponent const& comp2);
		kmo::Vector GetCollisionUnitVector(PhysicsComponent const& comp1, PhysicsComponent const& comp2) const;
	private:
		std::vector<PhysicsComponent*> m_registeredComponents;
		std::vector<PhysicsComponent*> m_movingComponents;
		std::vector<PhysicsComponent*> m_componentsToRemove;
		Notifier<kmo::CollisionEvent> m_notifier;
	};
}
