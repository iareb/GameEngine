#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API _declspec(dllexport)
	#else
		#define HAZEL_API _declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else
	#error GameEngine only support Windows!
#endif

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