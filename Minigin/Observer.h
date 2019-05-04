#pragma once
#include <vector>
#include <memory>

#include "ObserverDetacher.h"

namespace kmo
{
	template<typename EventType>
	class Notifier;

	template<typename EventType>
	class AbstractObserver
	{
		friend Notifier<EventType>;

	public:
		virtual ~AbstractObserver() = default;
		virtual void Observe(EventType const & event) = 0;

	private:
		// virtual ObserverDetacher<EventType>& GetDetacher() = 0;
		virtual void Attach(Notifier<EventType>& notifier) = 0;
		virtual void Detach(Notifier<EventType>& notifier) = 0;
	};


	template<typename EventType>
	class RigidUnsafeObserver : public AbstractObserver<EventType>
	{
	public:
		virtual ~RigidUnsafeObserver() = default;

	private:
		inline void Attach(Notifier<EventType>&) override {};
		inline void Detach(Notifier<EventType>&) override {};
	};


	template<typename EventType>
	class Observer : public AbstractObserver<EventType>
	{
	public:
		Observer()
			: m_pDetacher(std::make_unique<SingleSubjectDetacher<EventType>>()){}
		inline virtual ~Observer()
		{
			for(auto notifier : m_pDetacher->GetAttachedNotifiers())
			{
				notifier->Detach(*this);
			}
		}
	private:
		inline void Attach(Notifier<EventType>& notifier) override {
			if(m_pDetacher->HasReachedAttachCapacity())
			{
				m_pDetacher = std::make_unique<MultiSubjectDetacher<EventType>>(m_pDetacher->ConvertToMultiSubjectDetacher());
			}
			m_pDetacher->Attach(notifier);
		}
		inline void Detach(Notifier<EventType>& notifier) override
		{
			m_pDetacher->Detach(notifier);
		}
	private:
		// Could save half the memory in the most common case of single subject observer
		// with something like a variant or union that automatically detects the type
		// std::variant<SingleSubjectDetacher<EventType>, std::unique_ptr<ObserverDetacher<EventType>>> m_pDetacher;
		// Not true, SingleSubjectDetacher is larger than just its pointer because of the v-table
		std::unique_ptr<ObserverDetacher<EventType>> m_pDetacher;
	};


	template<typename EventType>
	class Notifier final
	{
	public:
		inline  ~Notifier()
		{
			for(auto observer : m_observers)
			{
				observer->Detach(*this);
			}
		}
		inline void Notify(EventType event)
		{
			for(auto observer : m_observers)
			{
				observer->Observe(event);
			}
		}
		inline void Attach(AbstractObserver<EventType>& observer)
		{
			m_observers.push_back(&observer);
			observer.Attach(*this);
		}
		inline void Detach(AbstractObserver<EventType>& observer)
		{
			// Only one occurence. Don't attach multiple times.
			auto it{ std::find(m_observers.begin(), m_observers.end(),  &observer) };
			if(it != m_observers.end())
			{
				m_observers.erase(it);
			}
		}

	private:
		std::vector<AbstractObserver<EventType>*> m_observers{};
	};
}

