#pragma once

namespace kmo
{
	class State
	{
	public:
		virtual ~State() = default;
		virtual void Update() = 0;
		virtual State& LateUpdate() = 0;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		// TODO: wrap the returned State in something and foresee an implicit conversion constructor
		// LateUpdate would then only return these wrapped states
		// This will prevent bugs were the raw state are returned without calling GoToState
		// Problem : more difficult to work with inheritance then
		virtual State& GoToState(State& state)
		{
			if(this != &state)
			{
				Exit();
				state.Enter();
			}
			return state;
		}

	};
}
