#pragma once
#undef near
#undef far
#include <glm/glm.hpp>

namespace SolEngine::Settings
{
    struct CameraSettings
    {
        bool      isMouseOverGUI    { false };  // Is the mouse hovering over any GUI windows currently?

        float     near              { 0.01f };
        float     far               { 1000.f };
        float     fieldOfViewDegrees{ 50.f };
        glm::vec3 position          { 0.f };
    };
}
