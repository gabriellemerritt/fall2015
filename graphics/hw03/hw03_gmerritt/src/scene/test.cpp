#include "camera.h"
#include <iostream> 
#include <cstdlib> 




int main(void)
{

	Camera test_cam; 


	std::cout << cam.right[0]; 
	return 0; 
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{" 
        << vec.x << " " << vec.y << " "<< vec.z 
        << "}";

    return out;
}