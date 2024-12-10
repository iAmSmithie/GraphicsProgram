#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"

class rayTracer
{
public:
	glm::vec3 TraceRay(ray _Intersect);
		void AddSphere(sphere* _sphere);

private:
	std::vector<sphere*> _spheres;
};