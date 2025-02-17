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
	#define HZ_ASSERTS(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERTS(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERTS(x, ...)
	#define HZ_CORE_ASSERTS(x, ...)
#endif

// Shift del bit 1 a sinistra di x posizioni. 
#define BIT(x) (1 << x)