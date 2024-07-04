#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "VertexTypes.h"

struct PlaneGeoData {
    glm::mat4& transform;
    glm::mat3& uvTransfrom;
};

/**
* Utilitiy class used to build a plane and add it to 
*/
class PlaneGeo {
private:

	PlaneGeo() {

	}
public:
    /**
    * The plane is create on the x,z plane center at (0,0). It's square from [-1,1] on the x and z axis.
    * Normal is positive z.
    */
    static void buildPlan(std::vector<VertexTextureColor>& vertices, std::vector<uint16_t>& indices, const PlaneGeoData& data);

};

