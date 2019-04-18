#include <iostream>
#pragma once
namespace kmo
{
	class Vector final
	{
	public:
		Vector() noexcept
			: Vector(0.f, 0.f){}
		Vector(float newX, float newY) noexcept
			: x(newX), y(newY) {}

		inline bool operator==(Vector const& rhs) const noexcept
		{
			return this->x == rhs.x && this->y == rhs.y;
		}
		inline bool operator!=(Vector const& rhs) const noexcept
		{
			return !this->operator==(rhs);
		}
		inline Vector& operator+=(Vector const& rhs) noexcept
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}
	public:
		float x;
		float y;
	};
}

inline std::ostream& operator<<(std::ostream& os, kmo::Vector const& m) { 
	return os << " " << m.x << ", " << m.y << " ";
}

inline kmo::Vector operator*(float scalar, kmo::Vector const& rhs) noexcept
{
	return kmo::Vector(scalar * rhs.x, scalar * rhs.y);
}

