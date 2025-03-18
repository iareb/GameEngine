#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "GameEngine/Events/Event.h"
#include "GameEngine/Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "GameEngine/Renderer/Shader.h"
#include "GameEngine/Renderer/Buffer.h"
#include "GameEngine/Renderer/VertexArray.h"

#include "GameEngine/Renderer/OrthographicCamera.h"

namespace GameEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		// Vogliamo richiamare il singleton di Application
		// da qualsiasi punto del programma.
		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}	