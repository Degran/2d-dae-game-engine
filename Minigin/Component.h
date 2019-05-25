#pragma once
namespace kmo
{
	class Component
	{
	public:
		virtual ~Component() = default;

		virtual void Update(float deltaTime) = 0;
		virtual void LateUpdate(float deltaTime) = 0;
	};
}
