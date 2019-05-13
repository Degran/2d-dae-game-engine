#pragma once
#include <iostream>
#include <exception>

namespace kmo
{
	class Vector;
	inline kmo::Vector operator/(kmo::Vector const& lhs, float scalar);
	inline kmo::Vector operator*(float scalar, kmo::Vector const& rhs) noexcept;

	class NormalizedZeroVectorException : public std::exception
	{};

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
		inline Vector operator+(Vector const& rhs) const noexcept
		{
			return Vector(x + rhs.x, y + rhs.y);
		}
		inline bool IsZero() const noexcept
		{
			return abs(x) < ZERO_THRESHOLD && abs(y) < ZERO_THRESHOLD;
		}
		inline float GetSquaredMagnitude() const noexcept
		{
			return x * x + y * y;
		}
		inline float GetMagnitude() const
		{
			return std::sqrt(GetSquaredMagnitude());
		}
		inline Vector GetNormalized() const throw(NormalizedZeroVectorException)
		{
			if(IsZero())
			{
				throw NormalizedZeroVectorException();
			}
			return *this / GetMagnitude();
		}
		inline float Dot(Vector const& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y;
		}
		inline Vector ProjectOntoVector(Vector const& other) const
		{
			Vector const normalized{ other.GetNormalized() };
			float const length{ Dot(normalized) };
			return length * normalized;
		}
	public:
		float x;
		float y;

		static float constexpr ZERO_THRESHOLD{ 0.00001f };
	};

	inline std::ostream& operator<<(std::ostream& os, kmo::Vector const& m) { 
		return os << " " << m.x << ", " << m.y << " ";
	}

	inline kmo::Vector operator*(float scalar, kmo::Vector const& rhs) noexcept
	{
		return kmo::Vector(scalar * rhs.x, scalar * rhs.y);
	}

	inline kmo::Vector operator/(kmo::Vector const& lhs, float scalar)
	{
		float const inverse{ 1.0f / scalar };
		return inverse * lhs;
	}
}
