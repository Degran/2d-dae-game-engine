#pragma once
#include <chrono>
#include <thread>
#include <memory>
#include <vector>
#include <algorithm>

#include "InputManager.h"
#include "PhysicsEngine.h"
#include "Component.h"
#include "SceneManager.h"

namespace kmo
{
	class Game final
		{
	public:
		inline void Loop()
		{
			m_previousStartTime = std::chrono::high_resolution_clock::now();
			// TODO: some kind of quit signal
			while(true)
			{
				SingleCycleStep();
			}
		}
		inline void SingleCycleStep()
		{
			UpdateLag();
			if (WaitIfTooFast()) { return; }
			m_inputManager.ProcessInput();
			while(m_updateDeltaTime <= m_lag)
			{
				PerformFixedTimeUpdate();
				m_lag -= m_updateDeltaTime;
			}
			float frameInterpolationRatio{ m_lag / m_updateDeltaTime };
			// Renderer.Render(frameInterpolationRatio)
		}
		inline void UpdateLag()
		{
			auto const currentStartTime{ std::chrono::high_resolution_clock::now() };
			auto const passedTime{ std::chrono::duration<float>(currentStartTime - m_previousStartTime) };
			m_previousStartTime = currentStartTime;
			m_lag += passedTime;
		}
		inline bool WaitIfTooFast() const
		{
			if(m_lag < m_updateDeltaTime)
			{
				std::this_thread::sleep_for(m_updateDeltaTime - m_lag);
				return true;
			}
			return false;
		}
		inline void PerformFixedTimeUpdate()
		{
			for (std::unique_ptr<Component> const& unique : m_looseComponents) { unique->Update(m_updateDeltaTime.count()); }
			m_sceneManager.Update(m_updateDeltaTime.count());
			m_physicsEngine.Update(m_updateDeltaTime.count());
			for (std::unique_ptr<Component> const& unique : m_looseComponents) { unique->LateUpdate(m_updateDeltaTime.count()); }
			m_sceneManager.LateUpdate(m_updateDeltaTime.count());
		}
		inline void SetMinimumFeasibleUpdateFps(float fps)
		{
			m_updateDeltaTime = std::chrono::duration<float, std::ratio<1, 1> >(1.f / fps);
		}
		inline void AddLooseComponent(std::unique_ptr<Component> component)
		{
			m_looseComponents.push_back(std::move(component));
		}
		inline void RemoveLooseComponent(Component const& component)
		{
			auto equalityLambda{ [&component](std::unique_ptr<Component> const& element) {return &component == element.get(); } };
			std::remove_if(m_looseComponents.begin(), m_looseComponents.end(), equalityLambda);
		}
		
	public:
		InputManager m_inputManager;
		PhysicsEngine m_physicsEngine;
		// Renderer
		SceneManager m_sceneManager;
	private:
		std::vector<std::unique_ptr<Component> > m_looseComponents;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_previousStartTime;
		std::chrono::duration<float, std::ratio<1, 1> > m_updateDeltaTime{ 1.f / 60.f };
		std::chrono::duration<float> m_lag{ 0.f };
	};
}
