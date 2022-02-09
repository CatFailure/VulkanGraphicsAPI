#include "MatrixUtility.hpp"

glm::mat4 SolEngine::Math::FastTransformMatrix(const glm::vec3 &position,
                                               const glm::vec3 &scale, 
                                               const glm::vec3 &rotation)
{
    const float cosRotX = glm::cos(rotation.x);
    const float cosRotY = glm::cos(rotation.y);
    const float cosRotZ = glm::cos(rotation.z);

    const float sinRotX = glm::sin(rotation.x);
    const float sinRotY = glm::sin(rotation.y);
    const float sinRotZ = glm::sin(rotation.z);

    // Corresponds to the Y(1), X(2), Z(3) Tait-Bryan angles matrix:
    // https://wikimedia.org/api/rest_v1/media/math/render/svg/315bca69e4cbda3218014b5a9b4878aa89231064
    return glm::mat4
    {
        {
            scale.x * (cosRotY * cosRotZ + sinRotY * sinRotX * sinRotZ),    // c1c3 + s1s2s3
            scale.x * (cosRotX * sinRotZ),                                  // c2s3
            scale.x * (cosRotY * sinRotX * sinRotZ - cosRotZ * sinRotY),    // c1s2s3 - c3s1
            0.0f,
        },
        {
            scale.y * (cosRotZ * sinRotY * sinRotX - cosRotY * sinRotZ),    // c3s1s2 - c1s3
            scale.y * (cosRotX * cosRotZ),                                  // c2c3
            scale.y * (cosRotY * cosRotZ * sinRotX + sinRotY * sinRotZ),    // c1c3s2 + s1s3
            0.0f,
        },
        {
            scale.z * (cosRotX * sinRotY),                                  // c2s1
            scale.z * (-sinRotX),                                           // -s2
            scale.z * (cosRotY * cosRotX),                                  // c1c2
            0.0f,
        },
        {
            position.x,
            position.y,
            position.z,
            1.0f
        }
    };
}
