#pragma once

#include <memory>

#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define _declspec(dllexport)
	#else
		#define _declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else 
	#define HAZEL_API
#endif
#else
	#error GameEngine only support Windows!
#endif

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif // HZ_DEBUG


#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

// Shift del bit 1 a sinistra di x posizioni. 
#define BIT(x) (1 << x)

// Creiamo un oggetto callable che, quando invocato, eseguirà OnEvent sull'oggetto this (l'istanza di Application) 
// e gli passerà il primo argomento (std::placeholders::_1) al posto del segnaposto.
#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace GameEngine {
	
	template<typename T>
	using Scope = std::unique_ptr<T>;

	// ad es. std::shared_ptr<Shader> <--> GameEngine::Ref<Shader>
	template<typename T>
	using Ref = std::shared_ptr<T>;

}