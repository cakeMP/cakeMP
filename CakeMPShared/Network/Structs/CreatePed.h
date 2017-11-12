#pragma once

namespace NetStructs
{
	struct CreatePed
	{
		uint32_t m_model = 0;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
	};
}
