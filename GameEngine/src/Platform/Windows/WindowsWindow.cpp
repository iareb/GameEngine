#include "hzpch.h"
#include "WindowsWindow.h"

#include "GameEngine/Events/KeyEvent.h"
#include "GameEngine/Events/MouseEvent.h"
#include "GameEngine/Events/ApplicationEvent.h"

#include <glad/glad.h>

namespace GameEngine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// Crea una finestra e un contesto OpenGL associato.
		// Il terzo argomento è il titolo della finestra.
		// Il quarto è GLFWmonitor *monitor. Indica il monitor da usare se vuoi una finestra fullscreen. Con nullptr ottieni una finestra standard. 
		// Il quinto è share. È un contesto OpenGL da condividere, se vuoi condividere risorse tra più finestre. Con nullptr, non condividi nessun contesto.
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		
		// Rende il contesto di m_Window il contesto OpenGL corrente sul thread che sta eseguendo la chiamata.
		glfwMakeContextCurrent(m_Window);	

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		// Associa un puntatore utente alla finestra. È un modo per salvare dati personalizzati all’interno della struttura GLFWwindow.
		glfwSetWindowUserPointer(m_Window, &m_Data);
		
		SetVSync(true);


		// Settiamo i callback di GLFW

		// Questa funzione di GLFW consente di registrare una funzione (o lambda) che verrà chiamata ogni volta che la finestra cambia dimensione.
		// Il secondo parametro è una lambda che accetta tre parametri: 
		// GLFWwindow* window: puntatore alla finestra in cui si è verificato il cambiamento.
		// int width e int height : le nuove dimensioni della finestra
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			// Questa funzione recupera un puntatore a dati utente associati alla finestra. 
			// Spesso viene usata per memorizzare informazioni personalizzate o per collegare la finestra a un oggetto di gestione.
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		// key: il codice del tasto premuto (o rilasciato). Ad esempio GLFW_KEY_A, GLFW_KEY_ESCAPE, ecc.
		// scancode: il codice di scansione del tasto (dipende dal sistema operativo / hardware). Può servire per differenziare layout di tastiera particolari.
		// action: indica se il tasto è stato premuto (GLFW_PRESS), rilasciato (GLFW_RELEASE) o ripetuto (GLFW_REPEAT).
		// mods: bitmask che indica i modifier keys (SHIFT, CTRL, ALT, ecc.) in quel momento. Ad esempio, se SHIFT era premuto, troverai GLFW_MOD_SHIFT.
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;	
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}


	void WindowsWindow::OnUpdate()
	{
		// Processa tutti gli eventi in coda (input da tastiera, mouse, eventi di finestra ecc.) e aggiorna lo stato interno di GLFW.
		// GLFW mantiene una coda di eventi che vengono generati dal sistema operativo. 
		// Quando chiami glfwPollEvents(), GLFW legge tutti gli eventi in coda e invoca eventuali callback registrati (es. glfwSetKeyCallback, ecc). 
		// Dopo questa chiamata, le funzioni glfwGetKey, glfwGetMouseButton e simili forniscono lo stato aggiornato dei dispositivi di input.
		glfwPollEvents();

		// Scambia i buffer front e back nel contesto OpenGL associato a m_Window.
		// In un contesto double-buffered, ci sono due buffer di disegno:
		// front buffer: quello visualizzato sullo schermo in questo momento.
		// back buffer: quello su cui stai effettivamente disegnando (rendering) in questo frame.
		// Quando si finisce di disegnare il frame, il back buffer diventa il front buffer (e quindi appare a schermo). 
		// Il vecchio front buffer diventa il back buffer, pronto per disegnare il frame successivo.
		// Questo evita lo “sfarfallio” (flickering) che succederebbe se disegnassi direttamente sul buffer visualizzato a schermo. 
		// Con il double buffering, disegni “dietro le quinte” e poi mostri tutto in un colpo solo quando il frame è completo.
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		// glfwSwapInterval: imposta l’intervallo di swap per il contesto OpenGL corrente, 
		// ovvero quante volte attendere il vertical blank (sincronizzazione verticale) prima di eseguire lo swap dei buffer.
		// Si può immaginare come il momento in cui il monitor finisce di aggiornare l’immagine di un frame e si prepara a disegnare il prossimo.
		// Il V-blank è la brevissima pausa tra la fine del disegno di un frame e l’inizio del successivo.
		// Il V-sync (Vertical Synchronization) è la tecnica che sincronizza lo swap dei buffer con il vertical blank del monitor.
		if (enabled)
			// interval = 1: V-sync abilitato. Sincronizza lo swap con ogni refresh (di solito 60 volte al secondo).
			glfwSwapInterval(1);
		else
			// interval = 0: V-sync disabilitato. Lo swap avviene subito, senza aspettare il monitor.
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}