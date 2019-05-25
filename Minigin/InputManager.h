#pragma once
#include <XInput.h>
#include <memory>
#include <vector>
#include "Singleton.h"

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

	class InputState
	{
		
	};

	class InputManager final
	{
	public:
		inline bool ProcessInput() {
			return true;
		}
		inline void SetInputSource(std::unique_ptr<InputSource> inputSource)
		{
			m_inputSource = std::move(inputSource);
		}
	private:
		std::unique_ptr<InputSource> m_inputSource;
	};
}
