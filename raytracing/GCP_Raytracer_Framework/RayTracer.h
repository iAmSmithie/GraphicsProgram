#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"

class rayTracer
{
public:
	glm::vec3 TraceRay(ray _Intersect);
		void AddSphere(sphere* _sphere);

        void ClearSpheres() {
            for (sphere* s : _spheres) {
                delete s; // Free the memory of dynamically allocated spheres
            }
            _spheres.clear(); // Clear the vector or list that holds spheres
        }
private:
	std::vector<sphere*> _spheres;
};