#pragma once
#include <memory>
#include <vector>

#include "Transform.h"
#include "Texture2D.h"
#include "SceneObject.h"

#include "Component.h"

namespace dae
{
	class GameObject : public SceneObject
	{
	public:
		void Update() override;
		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform mTransform;
		std::shared_ptr<Texture2D> mTexture;
	};
}

namespace kmo
{
	class GameObject final
	{
	public:
		static GameObject& GetNullGameObject()
		{
			static GameObject instance;
			return instance;
		}
		inline void Update(float deltaTime)
		{
			for(std::unique_ptr<Component> const& component : m_components)
			{
				component->Update(deltaTime);
			}
		}
		inline void LateUpdate(float deltaTime)
		{
			for(std::unique_ptr<Component> const& component : m_components)
			{
				component->LateUpdate(deltaTime);
			}
		}
		inline void AttachComponent(std::unique_ptr<Component> component)
		{
			m_components.push_back(std::move(component));
			component->AttachTo(*this);
		}
		template<class T>
		inline std::vector<T*> GetAllComponentsOfType()
		{
			std::vector<T*> result;
			for(std::unique_ptr<Component> const& unique_ptr : m_components)
			{
				if(typeid(*unique_ptr).hash_code() == typeid(T).hash_code())
				{
					result.push_back(unique_ptr.get());
				}
			}
			return result;
		}
	private:
		std::vector<std::unique_ptr<Component> > m_components;
	};
}