#pragma once
#include <vector>
#include "geometry/VertexTypes.h"
#include <glm/glm.hpp>

class GeoUtilities {
public:
    static void applyTransform(std::vector<VertexTextureColor>& vertex, const glm::mat4& transform, size_t startIndex = 0);
    static void applyTransformUvs(std::vector<VertexTextureColor>& vertex, const glm::mat3& transform, size_t startIndex = 0);
};

