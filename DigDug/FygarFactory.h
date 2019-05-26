#pragma once
#include "Game.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

namespace digdug
{
	class FygarFactory final
	{
	public:
		FygarFactory(kmo::Game& targetGame)
			: m_targetScene(targetGame.m_sceneManager)
			, m_physicsEngine(targetGame.m_physicsEngine){}
		inline void CreateFygar()
		{
			kmo::GameObject result;
			kmo::PhysicsInputData velocityData;
			auto tempPhysics{ std::make_unique<kmo::PhysicsComponent>(m_physicsEngine, velocityData) };
			result.AttachComponent(std::move(tempPhysics));
			m_targetScene.AddGameObject(std::move(result));
		}
	private:
		kmo::SceneManager& m_targetScene;
		kmo::PhysicsEngine& m_physicsEngine;
	};
}

