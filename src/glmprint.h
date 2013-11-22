#ifndef GLM_PRINT_STUFF
#define GLM_PRINT_STUFF
#include <glm/glm.hpp>

inline std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{" 
        << vec.x << " " << vec.y << " "<< vec.z 
        << "}";

    return out;
}

#endif
