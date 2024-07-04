#include "PlaneGeo.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "geometry/GeoUtilities.h"

void PlaneGeo::buildPlan(std::vector<VertexTextureColor>& vertices, std::vector<uint16_t>& indices, const PlaneGeoData& data) {

    uint32_t offset = (uint32_t)vertices.size();
    float d = 0;
    VertexTextureColor v0{};
    v0.pos = { -.5f, .5f, d };
    v0.texCoord = { 0, 0 };
    v0.color = { 1.0f, 1.0f, 1.0f };
    vertices.push_back(v0);

    VertexTextureColor v1{};
    v1.pos = { .5f, .5f, d };
    v1.texCoord = { 1, 0 };
    v1.color = { 1.0f, 1.0f, 1.0f };
    vertices.push_back(v1);

    VertexTextureColor v2{};
    v2.pos = { .5f, -.5f, d };
    v2.texCoord = { 1, 1 };
    v2.color = { 1.0f, 1.0f, 1.0f };
    vertices.push_back(v2);

    VertexTextureColor v3{};
    v3.pos = { -.5f, -.5f, d };
    v3.texCoord = { 0, 1 };
    v3.color = { 1.0f, 1.0f, 1.0f };
    vertices.push_back(v3);

    // apply the transfrom to the last 4
    GeoUtilities::applyTransform(vertices, data.transform, vertices.size() - 4);
    GeoUtilities::applyTransformUvs(vertices, data.uvTransfrom, vertices.size() - 4);

    indices.push_back(offset + 0);
    indices.push_back(offset + 1);
    indices.push_back(offset + 3);

    indices.push_back(offset + 1);
    indices.push_back(offset + 2);
    indices.push_back(offset + 3);
}

