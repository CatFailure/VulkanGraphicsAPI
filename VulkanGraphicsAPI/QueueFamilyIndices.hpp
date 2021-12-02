#pragma once

namespace SolEngine::Data
{
    struct QueueFamilyIndices
    {
        bool IsComplete()       const { return graphicsFamilyHasValue && presentFamilyHasValue; }
        bool AreFamiliesEqual() const { return graphicsFamily == presentFamily; }

        uint32_t graphicsFamily;
        uint32_t presentFamily;

        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue  = false;
    };
}
