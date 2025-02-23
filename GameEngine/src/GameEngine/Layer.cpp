#include "hzpch.h"
#include "Layer.h"

namespace GameEngine {

	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
		m_Deleted = true;
		std::cout << "Destroying Layer: " << this << std::endl;
	}

}