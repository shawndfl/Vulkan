#include "Log.h"

/**********************************************************************/
std::string getPath(const std::string& path) {
    size_t end = path.find_last_of("\\");
    if (end == std::string::npos) {
        return path;
    }
    return path.substr( end+1, path.length());
}

/**********************************************************************/
std::ostream& operator<<(std::ostream& out, const glm::vec3& v) {
    return out << v.x << ", " << v.y << ", " << v.z;
}

std::ostream& operator<<(std::ostream& out, const glm::vec2& v) {
    return out << v.x << ", " << v.y;
}