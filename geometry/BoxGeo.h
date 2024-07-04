#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "geometry/VertexTypes.h"

struct BoxGeoData {
    glm::mat4 transform;
    glm::mat3 uvTransfrom;
};

/**
* Creates a box and adds it to the vertices and indices
*/
class BoxGeo {
public:

    /**
     * cubes are build with their pivot points at the bottom center of the cube.
     */
    static void buildBox(std::vector<VertexTextureColor>& vertices, std::vector<uint16_t>& indices, const BoxGeoData& data);

};

