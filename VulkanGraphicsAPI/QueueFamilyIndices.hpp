#pragma once

namespace SolEngine::Data
{
    struct QueueFamilyIndices
    {
        bool IsComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
        bool AreFamiliesEqual() { return graphicsFamily == presentFamily; }

        uint32_t graphicsFamily;
        uint32_t presentFamily;

        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
    };
}
