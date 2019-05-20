#pragma once
#include "Vector.h"
#include <algorithm>

namespace kmo
{
	class Interval final
	{
	public:
		Interval() noexcept
			: Interval(0.f, 0.f) {}
		Interval(float bound1, float bound2) noexcept
			: m_lowerBound(min(bound1, bound2)), m_upperBound(max(bound1, bound2))
		{}
		inline bool IsOverlappingOrTouches(Interval const& other) const noexcept
		{
			return IsABoundInClosedInterval(other) || other.IsABoundInClosedInterval(*this);
		}
		inline bool IsStrictlyOverlapping(Interval const& other) const noexcept
		{
			return IsABoundInOpenInterval(other) || other.IsABoundInOpenInterval(*this);
		}
		inline bool IsInClosedInterval(float value) const noexcept
		{
			return m_lowerBound <= value && value <= m_upperBound;
		}
		inline constexpr bool IsInOpenInterval(float value) const noexcept
		{
			return m_lowerBound + STRICT_OVERLAP_MARGIN < value && value < m_upperBound - STRICT_OVERLAP_MARGIN;
		}
		inline bool IsTouchingInterval(float value) const noexcept
		{
			return value == m_lowerBound || m_upperBound == value;
		}
		inline bool IsTouchingInterval(Interval const& other) const noexcept
		{
			return other.m_upperBound == m_lowerBound || m_upperBound == other.m_lowerBound;
		}
	private:
		inline bool IsABoundInClosedInterval(Interval const& other) const noexcept
		{
			return other.IsInClosedInterval(m_lowerBound) || other.IsInClosedInterval(m_upperBound);
		}
		inline bool IsABoundInOpenInterval(Interval const& other) const noexcept
		{
			return other.IsInOpenInterval(m_lowerBound) || other.IsInOpenInterval(m_upperBound);
		}
	private:
		float m_lowerBound;
		float m_upperBound;
		static constexpr float STRICT_OVERLAP_MARGIN{ .0f };
	};


	class Box final
	{
	public:
		Box() noexcept
			: Box({0.f, 0.f}, {0.f, 0.f}) {}
		Box(kmo::Vector const& firstVector, kmo::Vector const& secondVector) noexcept
			: m_topLeftPoint( min(firstVector.x, secondVector.x)
				, min(firstVector.y, secondVector.y) )
			, m_bottomRightPoint( max(firstVector.x, secondVector.x)
				, max(firstVector.y, secondVector.y) )
		{}
		static Box ConstructBySquareSide(float squareSide) noexcept
		{
			return { { -squareSide / 2, -squareSide / 2 }, { squareSide / 2, squareSide / 2 } };
		}

		inline Box ConstructBoxTranslatedByVector(kmo::Vector vector) const noexcept
		{
			return Box(m_topLeftPoint + vector, m_bottomRightPoint + vector);
		}
		inline bool IsOverlapping(Box const& other) const noexcept
		{
			bool const isXOverlapping{ ConstructXInterval().IsOverlappingOrTouches(other.ConstructXInterval()) };
			bool const isYOverlapping{ ConstructYInterval().IsOverlappingOrTouches(other.ConstructYInterval()) };
			return isXOverlapping && isYOverlapping;
		}
		inline bool IsStrictlyOverlapping(Box const& other) const noexcept
		{
			bool const isXOverlapping{ ConstructXInterval().IsStrictlyOverlapping(other.ConstructXInterval()) };
			bool const isYOverlapping{ ConstructYInterval().IsStrictlyOverlapping(other.ConstructYInterval()) };
			return isXOverlapping && isYOverlapping;
		}
		inline Interval ConstructXInterval() const noexcept
		{
			return Interval(m_topLeftPoint.x, m_bottomRightPoint.x);
		}
		inline Interval ConstructYInterval() const noexcept
		{
			return Interval(m_topLeftPoint.y, m_bottomRightPoint.y);
		}

	private:
		Vector m_topLeftPoint;
		Vector m_bottomRightPoint;
	};
}
