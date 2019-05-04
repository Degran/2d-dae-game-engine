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
			return IsABoundInInterval(other) || other.IsABoundInInterval(*this);
		}
		inline bool IsInClosedInterval(float value) const noexcept
		{
			return m_lowerBound <= value && value <= m_upperBound;
		}
	private:
		inline bool IsABoundInInterval(Interval const& other) const noexcept
		{
			return other.IsInClosedInterval(m_lowerBound) || other.IsInClosedInterval(m_upperBound);
		}
	private:
		float m_lowerBound;
		float m_upperBound;
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
