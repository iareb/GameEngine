#include "hzpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace GameEngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_WindowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		// Rende il contesto di m_Window il contesto OpenGL corrente sul thread che sta eseguendo la chiamata.
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		HZ_CORE_INFO("OpenGL Info:");
		HZ_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		// Scambia i buffer front e back nel contesto OpenGL associato a m_Window.
		// In un contesto double-buffered, ci sono due buffer di disegno:
		// front buffer: quello visualizzato sullo schermo in questo momento.
		// back buffer: quello su cui stai effettivamente disegnando (rendering) in questo frame.
		// Quando si finisce di disegnare il frame, il back buffer diventa il front buffer (e quindi appare a schermo). 
		// Il vecchio front buffer diventa il back buffer, pronto per disegnare il frame successivo.
		// Questo evita lo “sfarfallio” (flickering) che succederebbe se disegnassi direttamente sul buffer visualizzato a schermo. 
		// Con il double buffering, disegni “dietro le quinte” e poi mostri tutto in un colpo solo quando il frame è completo.
		glfwSwapBuffers(m_WindowHandle);
	}
}