#include "hzpch.h"
#include "Application.h"

#include "GameEngine/Events/ApplicationEvent.h"
#include "GameEngine/Log.h"

namespace GameEngine {

	Application::Application() 
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		HZ_TRACE(e.ToString());

		while (true);
	}

}
