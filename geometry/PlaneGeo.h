#pragma once

#include<vector>
#include "VertexTypes.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

/**
* Utilitiy class used to build a plane and add it to 
*/
class PlaneGeo {
private:

	PlaneGeo() {

	}
public:

	static void buildPlan(std::vector<VertexTextureColor>& vertices, std::vector<uint16_t>& indices) {

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

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(3);

        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
	}

	static void applyTransform(std::vector<VertexTextureColor>& vertex, const glm::mat4& transform) {
		for(int i =0; i < vertex.size(); i++) {
            VertexTextureColor& v = vertex[i];
			glm::vec4 pos(v.pos, 1);
            v.pos = pos * transform;
            v.pos.x += transform[3].x;
            v.pos.y += transform[3].y;
            v.pos.z += transform[3].z;
		}
	}

};

