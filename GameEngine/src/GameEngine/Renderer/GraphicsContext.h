#pragma once

namespace GameEngine {

	// Classe base per il rendering context.
	class GraphicsContext 
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};

}