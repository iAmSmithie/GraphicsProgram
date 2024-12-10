#include <vector>
#include <glm/glm.hpp>
#include "Camera.h"

ray camera::GetRay(glm::ivec2 _windowPosi)
{
	ray ray;

	int screenX = _windowPosi.x;
	int screenY = _windowPosi.y;
	int screenZ = 0;

	ray.Origin = glm::vec3(screenX, screenY, screenZ);
	ray.Direction = glm::vec3(0.0f, 0.0f, -1.0f);

	return ray;
}