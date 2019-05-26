#pragma once
namespace kmo
{
	class GameObject;
	class Component
	{
		friend GameObject;
	public:
		virtual ~Component() = default;

		virtual void Update(float deltaTime) = 0;
		virtual void LateUpdate(float deltaTime) = 0;
	private:
		virtual void AttachTo(GameObject& parent) = 0;
	};
}
