#include "hzpch.h"
#include "Application.h"

#include "GameEngine/Log.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

// Creiamo un oggetto callable che, quando invocato, eseguirà OnEvent sull'oggetto this (l'istanza di Application) 
// e gli passerà il primo argomento (std::placeholders::_1) al posto del segnaposto.
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() 
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		// Se l'evento e è di tipo WindowCloseEvent, il dispatcher chiamerà OnWindowClose()
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		HZ_CORE_TRACE("{0}", e.ToString());
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}
