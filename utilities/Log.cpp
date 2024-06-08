#include "Log.h"

/**********************************************************************/
std::string getPath(const std::string& path) {
    size_t end = path.find_last_of("\\");
    if (end == std::string::npos) {
        return path;
    }
    return path.substr( end+1, path.length());
}