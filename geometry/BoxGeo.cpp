#include "BoxGeo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "geometry/PlaneGeo.h"
#include "geometry/GeoUtilities.h"

/**********************************************************************/
void BoxGeo::buildBox(std::vector<VertexTextureColor>& verts, std::vector<uint16_t>& indices, const BoxGeoData& data) {
  
    float scale = 1;
    glm::mat4 transform;
    glm::mat3 uvTransform = glm::mat3(1.0f);
    size_t startIndex = verts.size();

    // front
    transform = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(scale));
    transform[3] = glm::vec4(0, .5f, .5f, 1);
    PlaneGeoData frontData{ transform, uvTransform};
    PlaneGeo::buildPlan(verts, indices, frontData);

    // back
    transform = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(1, 0, 0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform[3] = glm::vec4(0, .5f, -.5f, 1);
    PlaneGeoData backData{ transform, uvTransform };
    PlaneGeo::buildPlan(verts, indices, backData);

    // right
    transform = glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform[3] = glm::vec4(.5f, .5f, 0, 1);
    PlaneGeoData rightData{ transform, uvTransform };
    PlaneGeo::buildPlan(verts, indices, rightData);

    // left
    transform = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform[3] = glm::vec4(-.5f, .5f, 0, 1);
    PlaneGeoData leftData{ transform, uvTransform };
    PlaneGeo::buildPlan(verts, indices, leftData);

    // top
    transform = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform[3] = glm::vec4(0, 1.f, 0, 1);
    PlaneGeoData topData{ transform, uvTransform };
    PlaneGeo::buildPlan(verts, indices, topData);

    // bottom
    transform = glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1, 0, 0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform[3] = glm::vec4(0, 0, 0, 1);
    PlaneGeoData bottomData{ transform, uvTransform };
    PlaneGeo::buildPlan(verts, indices, bottomData);

    // apply the box transforms
    GeoUtilities::applyTransform(verts, data.transform, startIndex);
    GeoUtilities::applyTransformUvs(verts, data.uvTransfrom, startIndex);
}