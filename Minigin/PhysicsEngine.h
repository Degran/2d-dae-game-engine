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
	private:
		std::vector<PhysicsComponent*> m_registeredComponents;
		Notifier<kmo::CollisionEvent> m_notifier;
	};
}
