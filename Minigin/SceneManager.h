#pragma once
#include <vector>
#include <memory>
#include <algorithm>

#include "Singleton.h"
#include "GameObject.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene & CreateScene(const std::string& name);

		void Update();
		void Render();

	private:
		std::vector<std::shared_ptr<Scene>> mScenes;
	};

}

namespace kmo
{
	class SceneManager final : public dae::Singleton<SceneManager>
	{
	public:
		inline void AddGameObject(std::unique_ptr<GameObject> object)
		{
			m_objects.push_back(std::move(object));
		}
		inline void ScheduleForRemoval(GameObject const& object)
		{
			m_forRemoval.push_back(&object);
		}
		inline void Update(float deltaTime)
		{
			m_forRemoval.clear();
			for(std::unique_ptr<GameObject> const& object : m_objects)
			{
				object->Update(deltaTime);
			}
		}
		inline void LateUpdate(float deltaTime)
		{
			for(std::unique_ptr<GameObject> const& object : m_objects)
			{
				object->LateUpdate(deltaTime);
			}
			RemoveObjects();
		}
	private:
		inline void RemoveObjects()
		{
			for(auto ptr : m_forRemoval)
			{
				auto equalityLambda{ [&ptr](std::unique_ptr<GameObject> const& element) { return element.get() == ptr; } };
				std::remove_if(m_objects.begin(), m_objects.end(), equalityLambda);
			}
		}
	private:
		std::vector<std::unique_ptr<GameObject> > m_objects;
		std::vector<GameObject const*> m_forRemoval;
	};
}
