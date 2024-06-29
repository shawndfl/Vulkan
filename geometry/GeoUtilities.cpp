#include "GeoUtilities.h"

void GeoUtilities::applyTransform(std::vector<VertexTextureColor>& vertex, const glm::mat4& transform, size_t startIndex) {
    if (transform == glm::mat4(1)) {
        return;
    }
    for (int i = startIndex; i < vertex.size(); i++) {
        VertexTextureColor& v = vertex[i];
        glm::vec4 pos(v.pos, 1);
        v.pos = pos * transform;
        v.pos.x += transform[3].x;
        v.pos.y += transform[3].y;
        v.pos.z += transform[3].z;
    }
}