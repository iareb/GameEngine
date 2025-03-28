#pragma once

#include "GameEngine/Core.h"
#include "GameEngine/Core/Timestep.h"
#include "GameEngine/Events/Event.h"

namespace GameEngine {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	
	protected:
		std::string m_DebugName;
		bool m_Deleted = false;
	};

}
