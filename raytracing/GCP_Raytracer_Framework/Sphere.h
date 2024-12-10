#include <glm/glm.hpp>
#include "Ray.h"
#ifndef SPHERE_H
#define SPHERE_H

class sphere
{
public:
	sphere(glm::vec3 origin, float radius);
	~sphere();

	bool RayIntersect(ray _Intersect, glm::vec3& _rayPosi);
	glm::vec3 Shader(glm::vec3 _intersectPoint);
	glm::vec3 GetNormal(glm::vec3 _QueryPoint);

private:
	glm::vec3 _position;
	float _radius;
};

#endif