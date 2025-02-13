#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern GameEngine::Application* GameEngine::CreateApplication();

int main(int argc, char** argv)
{

	GameEngine::Log::Init();
	HZ_CORE_WARN("Initialized log");
	int a = 5;
	HZ_INFO("Hello Var={0}", a);

	auto app = GameEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif