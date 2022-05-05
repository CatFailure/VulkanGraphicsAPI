#pragma once
#undef near
#undef far
#include <glm/glm.hpp>

#include "ISerializable.hpp"
#include "Helpers.hpp"

using namespace Utility;
using namespace SolEngine::Interface;

namespace SolEngine::Settings
{
    struct CameraSettings : public ISerializable
    {
        virtual bool Deserialize(const rapidjson::Value& obj) override
        {
            // Are all Members present?
            if (!JsonHasMembers(obj,
                                PROP_NEAR,
                                PROP_FAR,
                                PROP_FOV, 
                                PROP_POSITION_OBJ))
            {
                DBG_ASSERT_MSG(false, "[CameraSettings] Object Missing Members!");

                return false;
            }
            near               = obj[PROP_NEAR].GetFloat();
            far                = obj[PROP_FAR].GetFloat();
            fieldOfViewDegrees = obj[PROP_FOV].GetFloat();

            // Construct Position Vector
            const auto positionObject = obj[PROP_POSITION_OBJ].GetObj();

            // Are all position Members present?
            if (!JsonHasMembers(positionObject,
                                PROP_POSITION_X, 
                                PROP_POSITION_Y, 
                                PROP_POSITION_Z))
            {
                DBG_ASSERT_MSG(false, "[CameraSettings] Position Object Missing Members!");

                return false;
            }

            position = 
            { 
                positionObject[PROP_POSITION_X].GetFloat(),
                positionObject[PROP_POSITION_Y].GetFloat(), 
                positionObject[PROP_POSITION_Z].GetFloat() 
            };

            return true;    // OK!
        }

        bool      isMouseOverGUI    { false };  // Is the mouse hovering over any GUI windows currently?

        float     near              { 0.01f };
        float     far               { 1000.f };
        float     fieldOfViewDegrees{ 50.f };
        glm::vec3 position          { 0.f, 0.f, 55.f };

    private:
        static constexpr const char* PROP_NEAR           { "near" };
        static constexpr const char* PROP_FAR            { "far" };
        static constexpr const char* PROP_FOV            { "field_of_view" };
        static constexpr const char* PROP_POSITION_OBJ   { "position" };
        static constexpr const char* PROP_POSITION_X     { "x" };
        static constexpr const char* PROP_POSITION_Y     { "y" };
        static constexpr const char* PROP_POSITION_Z     { "z" };
    };
}
