#pragma once
#include <vector>

namespace kmo
{
	template<typename  EventType>
	class Notifier;
	template<typename EventType>
	class MultiSubjectDetacher;

	template<typename EventType>
	class ObserverDetacher
	{
	public:
		virtual ~ObserverDetacher() = default;
		virtual void Attach(Notifier<EventType>& notifier) = 0;
		virtual void Detach(Notifier<EventType>& notifier) = 0;
		virtual bool HasReachedAttachCapacity() const = 0;
		virtual std::vector<Notifier<EventType>*> GetAttachedNotifiers() = 0;
		virtual MultiSubjectDetacher<EventType> ConvertToMultiSubjectDetacher() const = 0;
	};


	template<typename EventType>
	class MultiSubjectDetacher final : public ObserverDetacher<EventType>
	{
	public:
		inline void Attach(Notifier<EventType>& notifier) override
		{
			m_attachedNotifiers.push_back(&notifier);
		}
		inline void Detach(Notifier<EventType>& notifier) override
		{
			// Only one occurence. Don't attach multiple times.
			auto it{ std::find(m_attachedNotifiers.begin(), m_attachedNotifiers.end(), &notifier) };
			if(it != m_attachedNotifiers.end())
			{
				m_attachedNotifiers.erase(it);
			}
		}
		inline bool HasReachedAttachCapacity() const override { return false; }
		inline std::vector<Notifier<EventType>*> GetAttachedNotifiers() override
		{
			return m_attachedNotifiers;
		}
		inline MultiSubjectDetacher<EventType> ConvertToMultiSubjectDetacher() const override
		{
			return *this;
		}
	private:
		std::vector<Notifier<EventType>*> m_attachedNotifiers;
	};


	template<typename EventType>
	class NullObserverDetacher final : public ObserverDetacher<EventType>
	{
	public:
		static NullObserverDetacher<EventType>& GetDetacher()
		{
			static NullObserverDetacher result;
			return result;
		}

		inline void Attach(Notifier<EventType>&) override {}
		inline void Detach(Notifier<EventType>&) override {}
		inline bool HasReachedAttachCapacity() const override { return true; }
		inline std::vector<Notifier<EventType>*> GetAttachedNotifiers() override { return {}; }

		inline MultiSubjectDetacher<EventType> ConvertToMultiSubjectDetacher() const override
		{
			return MultiSubjectDetacher<EventType>();
		}
	};


	template<typename EventType>
	class SingleSubjectDetacher final : public ObserverDetacher<EventType>
	{
	public:
		inline void Attach(Notifier<EventType>& notifier) override
		{
			m_pNotifier = &notifier;
		}
		inline void Detach(Notifier<EventType>& notifier) override
		{
			if( m_pNotifier == &notifier)
			{
				m_pNotifier = nullptr;
			}
		}
		inline bool HasReachedAttachCapacity() const override
		{
			return m_pNotifier != nullptr;
		}
		inline std::vector<Notifier<EventType>*> GetAttachedNotifiers() override
		{
			if( m_pNotifier == nullptr)
			{
				return {};
			}
			return { m_pNotifier };
		}
		inline MultiSubjectDetacher<EventType> ConvertToMultiSubjectDetacher() const override
		{
			MultiSubjectDetacher<EventType> result;
			if(HasReachedAttachCapacity())
			{
				result.Attach(*m_pNotifier);
			}
			return result;
		}
	private:
		Notifier<EventType>* m_pNotifier{ nullptr };
	};
}
