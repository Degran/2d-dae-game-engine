#pragma once

namespace km
{
	class Component abstract
	{
	public:
		Component() = delete;
		virtual ~Component() = delete;

		virtual void Update() = 0;
	};
}
