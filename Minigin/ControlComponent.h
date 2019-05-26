#pragma once
#include "Vector.h"

namespace kmo
{
	class PhysicsInput
	{
	public:
		virtual ~PhysicsInput() = default;
	public:
		virtual kmo::Vector GetVelocity() const = 0;
	};

	class PhysicsInputData final : public PhysicsInput
	{
	public:
		inline kmo::Vector GetVelocity() const override
		{
			return m_velocity;
		}
	public:
		kmo::Vector m_velocity;
	};

	class NullPhysicsInput final : public PhysicsInput
	{
	public:
		static NullPhysicsInput const& GetInstance()
		{
			static NullPhysicsInput result;
			return result;
		}
		inline kmo::Vector GetVelocity() const override
		{
			return { 0.f, 0.f };
		}
	};

	class ControlComponent final
	{
	public:
		static Vector const& NullPosition()
		{
			static Vector instance;
			return instance;
		}
		inline PhysicsInput const& GetVelocity() const
		{
			return m_velocityData;
		}
	private:
		PhysicsInputData m_velocityData;
	};
}
