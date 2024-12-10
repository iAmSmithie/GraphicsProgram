#include <vector>
#include <glm/glm.hpp>
#include "RayTracer.h"
#include "Sphere.h"
#include "Ray.h"

glm::vec3 rayTracer::TraceRay(ray _Intersect)
{
	glm::vec3 ClsIntersectPoint;
	sphere* ClosestSphere = nullptr;
	float ClosestDistance = 1000000.0f;
	for (int i = 0; i < _spheres.size(); i++)
	{
		if (_spheres[i]->RayIntersect(_Intersect, ClsIntersectPoint))
		{
			float distance = glm::length(ClsIntersectPoint - _Intersect.Origin);
			//if the distance to the intersection point is less than the distance to the closest intersection point
			if (ClosestSphere == nullptr)
			{
				ClosestSphere = _spheres[i];
				ClosestDistance = distance;
			}
			else if (distance < ClosestDistance)
			{
				ClosestSphere = _spheres[i];
				ClosestDistance = distance;
			}
		} 
	}
	if (ClosestSphere == nullptr)
	{
		return glm::vec3(0.1f, 0.1f, 0.3f);
	}
	return ClosestSphere->Shader(ClsIntersectPoint);
}

void rayTracer::AddSphere(sphere* _sphere)
{
	_spheres.push_back(_sphere);
}