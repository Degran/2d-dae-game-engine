#pragma once
#include "Game.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "Vector.h"
#include "ObjectType.h"
#include "FygarController.h"

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
			auto tempController{ std::make_unique<FygarController>() };
			auto tempPhysics{ std::make_unique<kmo::PhysicsComponent>(m_physicsEngine, tempController->GetPhysicsInputData()) };
			tempPhysics->SetPosition(position);
			tempPhysics->SetHitboxSide(HIT_BOX_SIDE);
			tempPhysics->m_objectTypeCode = ObjectType::FYGAR;
			tempController->m_position = std::ref(tempPhysics->GetPosition());
			result.AttachComponent(std::move(tempPhysics));
			result.AttachComponent(std::move(tempController));
			m_targetScene.AddGameObject(std::move(result));
		}
	private:
		kmo::SceneManager& m_targetScene;
		kmo::PhysicsEngine& m_physicsEngine;

		static float constexpr HIT_BOX_SIDE{ 30.f };
	};
}

