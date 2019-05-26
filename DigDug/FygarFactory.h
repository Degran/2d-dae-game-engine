#pragma once
#include "SceneManager.h"

namespace digdug
{
	class FygarFactory final
	{
	public:
		FygarFactory(kmo::SceneManager& sceneManager)
			: m_targetScene(sceneManager){}
		inline void CreateFygar()
		{
			
		}
	private:
		kmo::SceneManager& m_targetScene;
	};
}

