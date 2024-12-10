#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include <iostream>

sphere::sphere(glm::vec3 origin, float radius)
{
	_position = origin;
	_radius = radius;
}

sphere::~sphere()
{
}

bool sphere::RayIntersect(ray _Intersect, glm::vec3& _rayPosi)
{
	glm::vec3 A = _Intersect.Origin;
	glm::vec3 N = glm::normalize(_Intersect.Direction);
	glm::vec3 P = _position;
	float R = _radius;

	float D = glm::length(P - A - (glm::dot(P - A, N)) * N);
	if (D > R)
	{
		return false;
	}
	float X = glm::sqrt(R * R - D * D);
	glm::vec3 C = A + (glm::dot(P - A, N) - X)* N;
    _rayPosi = C;
	return true;
}

glm::vec3 sphere::Shader(glm::vec3 _intersectPoint)
{
	glm::vec3 lightPosi = glm::vec3(500.0f, 100.0f, 100.0f);
	glm::vec3 Colour = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 lightDir = glm::normalize(lightPosi - _intersectPoint);
	glm::vec3 normal = GetNormal(_intersectPoint);
	glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	float specular = glm::pow(glm::max(glm::dot(lightDir, normal), 0.0f), 32);
	float diffuse = glm::max(glm::dot(lightDir, normal), 0.0f);
	glm::vec3 specuColour = glm::vec3(1, 1, 1) * specular;
	glm::vec3 diffuColour = glm::vec3(1, 1, 1) * diffuse;

	glm::vec3 L = ambient + diffuColour * Colour + specuColour;

	return L;
}

glm::vec3 sphere::GetNormal(glm::vec3 _QueryPoint)
{
	glm::vec3 normal = glm::normalize(_QueryPoint - _position);

	return normal;
}
