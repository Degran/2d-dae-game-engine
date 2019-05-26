#pragma once
#include "Game.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "Vector.h"
#include "ObjectType.h"

namespace digdug
{
	class FygarFactory final
	{
	public:
		FygarFactory(kmo::Game& targetGame)
			: m_targetScene(targetGame.m_sceneManager)
			, m_physicsEngine(targetGame.m_physicsEngine){}
		inline void CreateFygar(kmo::Vector position = {0.f, 0.f})
		{
			kmo::GameObject result;
			kmo::PhysicsInputData velocityData;
			auto tempPhysics{ std::make_unique<kmo::PhysicsComponent>(m_physicsEngine, velocityData) };
			tempPhysics->SetPosition(position);
			tempPhysics->SetHitboxSide(HIT_BOX_SIDE);
			tempPhysics->m_objectTypeCode = ObjectType::FYGAR;
			result.AttachComponent(std::move(tempPhysics));
			m_targetScene.AddGameObject(std::move(result));
		}
	private:
		kmo::SceneManager& m_targetScene;
		kmo::PhysicsEngine& m_physicsEngine;

		static float constexpr HIT_BOX_SIDE{ 30.f };
	};
}

