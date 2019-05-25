#pragma once
#include <XInput.h>
#include <memory>
#include <vector>
#include <map>
#include "Singleton.h"

#include "State.h"

namespace dae
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
	private:
		XINPUT_STATE currentState{};
	};

}

namespace kmo
{
	class InputEvent final
	{
	public:
		enum class DeviceType
		{
			CONTROLLER,
			KEYBOARD
		};
		inline bool operator<(InputEvent const& rhs) const
		{
			if(m_type == rhs.m_type)
			{
				if(m_deviceId == rhs.m_deviceId)
				{
					return m_keyCode < rhs.m_keyCode;
				}else{
					return m_deviceId < rhs.m_deviceId;
				}
			}else
			{
				return m_type == DeviceType::CONTROLLER;
			}
		}
	public:
		DeviceType m_type{ DeviceType::CONTROLLER };
		char m_deviceId{ 0 };
		int m_keyCode{ 0x0000 };
	};

	class InputSource
	{
	public:
		virtual ~InputSource() = default;
		virtual std::vector<InputEvent> GetInputEvents() = 0;
		virtual void ListenForEvent(InputEvent const& event) = 0;
	};

	class InputState;
	class InputCommand
	{
	public:
		virtual ~InputCommand() = default;
		virtual InputState& ExecuteFromStateToState(InputState& currentState) = 0;
	};

	class InputState: public State
	{
	public:
		virtual void Update() override = 0;
		virtual InputState& LateUpdate() override = 0;
		InputState& GoToState(State& state) override { return static_cast<InputState&>(State::GoToState(state)); }
		virtual void HandleInput(InputEvent const&) = 0;
		virtual void AssignCommandToInput(std::unique_ptr<InputCommand> command, InputEvent const& input) = 0;
	};

	class NullInputState final : public InputState
	{
	public:
		static NullInputState& GetInstance()
		{
			static NullInputState instance;
			return instance;
		}
		inline void Enter() override {}
		inline void Exit() override {}
		inline void Update() override {}
		inline InputState& LateUpdate() override { return GetInstance(); }
		inline void HandleInput(InputEvent const&) override {}
		inline void AssignCommandToInput(std::unique_ptr<InputCommand>, InputEvent const&) override{}
	};

	class StandardInputState final : public InputState
	{
	public:
		StandardInputState()
			: m_nextState(std::ref(*this)){}
		inline void Enter() override { m_nextState = std::ref(*this); }
		inline void Exit() override {}
		inline void Update() override{}
		inline InputState& LateUpdate() override { return GoToState(m_nextState.get()); };
		inline void HandleInput(InputEvent const& input) override
		{
			std::unique_ptr<InputCommand>& assigned{ m_assignedKeys[input] };
			if (!assigned) { return; }
			InputState& nextState{ assigned->ExecuteFromStateToState(*this) };
			if(&nextState != this)
			{
				m_nextState = std::ref(nextState);
			}
		}
		inline void AssignCommandToInput(std::unique_ptr<InputCommand> command, InputEvent const& input) override
		{
			m_assignedKeys[input] = std::move(command);
		}
	private:
		std::reference_wrapper<InputState> m_nextState;
		std::map<InputEvent, std::unique_ptr<InputCommand> > m_assignedKeys;
	};

	class InputManager final
	{
	public:
		InputManager(std::unique_ptr<InputState> startState)
			: m_currentState(std::ref(*startState))
		{
			AddInputState(std::move(startState));
		}
		InputManager()
			: m_currentState(NullInputState::GetInstance()){}
		inline void ProcessInput() {
			m_currentState.get().Update();
			ProcessAllInputsFromSource();
			m_currentState = std::ref(m_currentState.get().LateUpdate());
		}
		inline void ProcessAllInputsFromSource() const
		{
			for(InputEvent const input : m_inputSource->GetInputEvents())
			{
				m_currentState.get().HandleInput(input);
			}
		}
		inline void SetInputSource(std::unique_ptr<InputSource> inputSource)
		{
			m_inputSource = std::move(inputSource);
		}
		inline void AddInputState(std::unique_ptr<InputState> state)
		{
			m_states.push_back(std::move(state));
		}
	private:
		std::unique_ptr<InputSource> m_inputSource;
		std::reference_wrapper<InputState> m_currentState;
		std::vector<std::unique_ptr<InputState> > m_states;
	};
}
