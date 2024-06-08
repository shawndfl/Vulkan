#pragma once
#include <vector>
#include <string>

/**
* Manages assets remotally or from the file system
*/
class Assets {
public:
    static std::vector<char> readFile(const std::string& filename);
};

