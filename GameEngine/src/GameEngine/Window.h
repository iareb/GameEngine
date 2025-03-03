#pragma once

#include "hzpch.h"

#include "GameEngine/Core.h"
#include "GameEngine/Events/Event.h"

namespace GameEngine {

	// Proprietà della finestra.
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hazel Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}

	};

	// Interfaccia che fornisce una rappresentazione astratta di una finestra
	// indipendente dalla piattaforma. Le finestre si riferiscono a sistemi basati su desktop.
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Attributi della finestra
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// Ritorna la finestra di GLFW
		virtual void* GetNativeWindow() const = 0;

		// L'implementazione di questa funzione sarà diversa per piattaforma.
		static Window* Create(const WindowProps& props = WindowProps());
	};

}