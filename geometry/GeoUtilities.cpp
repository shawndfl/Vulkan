#include "GeoUtilities.h"

/**********************************************************************/
void GeoUtilities::applyTransform(std::vector<VertexTextureColor>& vertex, const glm::mat4& transform, size_t startIndex) {
    if (transform == glm::mat4(1)) {
        return;
    }
    for (size_t i = startIndex; i < vertex.size(); i++) {
        VertexTextureColor& v = vertex[i];
        glm::vec4 pos(v.pos, 1);
        v.pos = pos * transform;
        v.pos.x += transform[3].x;
        v.pos.y += transform[3].y;
        v.pos.z += transform[3].z;
    }
}

/**********************************************************************/
void GeoUtilities::applyTransformUvs(std::vector<VertexTextureColor>& vertex, const glm::mat3& transform, size_t startIndex) {
    if (transform == glm::mat3(1)) {
        return;
    }
    for (size_t i = startIndex; i < vertex.size(); i++) {
        VertexTextureColor& v = vertex[i];
        glm::vec3 point(v.texCoord, 1);
        v.texCoord = point * transform;
        v.texCoord.x += transform[2].x;
        v.texCoord.y += transform[2].y;
    }

}