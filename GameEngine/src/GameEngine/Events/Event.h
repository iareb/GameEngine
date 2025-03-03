#pragma once

#include "GameEngine/Core.h"

namespace GameEngine {

	/*
	* Quando si verifica un evento, questo viene immediatamente gestito 
	* e deve essere gestito subito e sul posto.
	*/
	
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Le categorie aiutano a filtrare gli eventi ricevuti.
	// Una motivazione dell'uso del bit field è che possiamo applicare 
	// categorie multiple a un singolo evento.
	// Possiamo avere bit multipli settati, dovremo poi applicare una mask
	// per vedere a quale categoriea/e appartiene l'evento.
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication  =	 BIT(0),
		EventCategoryInput		  =  BIT(1),
		EventCategoryKeyboard	  =  BIT(2),
		EventCategoryMouse		  =  BIT(3),
		EventCategoryMouseButton  =  BIT(4)
	};


// EventType::##type viene trasformato dal preprocessore in EventType::TypeName
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	// Classe base per gli eventi.
	class Event
	{
		friend class EventDispatcher;
	
	public:
		// Controlliamo se l'evento è stato gestito o no.
		bool Handled = false;

		// Queste 3 funzioni sono virtuali pure: devono essere implementate
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		// Si può effettuare l'override per ottenere informazioni più precise
		// o per stamparle in modo personalizzato.
		virtual std::string ToString() const { return GetName(); }

		// Controlla se l'evento ricevuto è della categoria passata come argomento.
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	};


	// Smista (dispatch) un evento basandosi sul suo tipo.
	class EventDispatcher
	{
		template<typename T>
		
		// EventFn<T> è un alias per una funzione che accetta un riferimento a un oggetto di tipo T e restituisce un bool.
		// In pratica, è un callback che dirà se l'evento è stato "gestito" o meno.
		using EventFn = std::function<bool(T&)>;

	public:
		// Riceviamo l'evento come reference (&), potrebbe essere qualsiasi tipo di evento.
		// Il riferimento è memorizzato in m_Event.
		// m_Event sarà l'evento che il dispatcher tenterà di "smistare" al gestore giusto.
		EventDispatcher(Event& event)
			: m_Event(event)
		{

		}

		// Questo metodo è un template, quindi può essere chiamato con qualsiasi tipo di evento.
		// Ad esempio: MouseMovedEvent, KeyPressedEvent, ecc.
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			// Verifica se il tipo runtime dell'evento (m_Event.GetEventType()) 
			// corrisponde al tipo statico del tipo di evento T passato come parametro.
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				// Se i tipi corrispondono, fa un cast forzato da Event& a T& usando un cast C-style:
				// *(T*)&m_Event → tratta m_Event come un riferimento a T(tipo specifico dell'evento).
				// Chiama la funzione handler func passando l'evento castato.
				// Memorizza il risultato in m_Handled, indicandone l'elaborazione.
				m_Event.Handled = func(*(T*)&m_Event);
				
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};


	// Overload dell'operatore <<. Utile per la libreria di logging.
	// Facilita chiamare ToString sull'evento.
	// Ad es. scrivendo Event myEvent; std::cout << myEvent;
	// verrà chiamata questa funzione.
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}