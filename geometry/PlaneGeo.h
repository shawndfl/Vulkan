#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "VertexTypes.h"
#include "geometry/GeoUtilities.h"

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
	static void buildPlan(std::vector<VertexTextureColor>& vertices, std::vector<uint16_t>& indices, const glm::mat4& transform, const glm::mat3& uvTransfrom) {

        uint32_t offset = (uint32_t) vertices.size();
        float d = 0;
        float s = 1.0;
        VertexTextureColor v0{};
        v0.pos = { -1 * s, 1 * s, d };
        v0.texCoord = { 0, 0 };
        v0.color = { 1.0f, 1.0f, 1.0f };
        vertices.push_back(v0);

        VertexTextureColor v1{};
        v1.pos = { 1 * s, 1 * s, d };
        v1.texCoord = { 1, 0 };
        v1.color = { 1.0f, 1.0f, 1.0f };
        vertices.push_back(v1);

        VertexTextureColor v2{};
        v2.pos = { 1 * s, -1 * s, d };
        v2.texCoord = { 1, 1 };
        v2.color = { 1.0f, 1.0f, 1.0f };
        vertices.push_back(v2);

        VertexTextureColor v3{};
        v3.pos = { -1 * s, -1 * s, d };
        v3.texCoord = { 0, 1 };
        v3.color = { 1.0f, 1.0f, 1.0f };
        vertices.push_back(v3);

        // apply the transfrom to the last 4
        GeoUtilities::applyTransform(vertices, transform, vertices.size() - 4);
        GeoUtilities::applyTransformUvs(vertices, uvTransfrom, vertices.size() - 4);

        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 3);

        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
	}

};

