#pragma once
#include "Vector.h"
#include <algorithm>

namespace kmo
{
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
			return { { squareSide / 2, squareSide / 2 }, { squareSide / 2, squareSide / 2 } };
		}

		inline Box ConstructBoxTranslatedByVector(kmo::Vector vector) const noexcept
		{
			return Box(m_topLeftPoint + vector, m_bottomRightPoint + vector);
		}

	private:
		Vector m_topLeftPoint;
		Vector m_bottomRightPoint;
	};
}
