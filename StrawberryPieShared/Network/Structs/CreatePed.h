#pragma once

#include <Network/NetHandle.h>

namespace NetStructs
{
	struct CreatePed
	{
		NetHandle m_handle;

		uint32_t m_model = 0;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
	};
}
