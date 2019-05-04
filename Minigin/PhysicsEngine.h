#pragma once
#include <vector>
#include "Observer.h"

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
		inline void RequestPositionUpdate(PhysicsComponent& component)
		{
			m_movingComponents.push_back(&component);
		}

		inline void Update(float deltaTime)
		{
			CheckCollisions(deltaTime);
		}
	private:
		void CheckCollisions(float deltaTime);
	private:
		std::vector<PhysicsComponent*> m_registeredComponents;
		std::vector<PhysicsComponent*> m_movingComponents;
		Notifier<kmo::CollisionEvent> m_notifier;
	};
}
