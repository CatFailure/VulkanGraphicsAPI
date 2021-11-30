#include "pch.hpp"
#include "SolVulkanWindow.hpp"

namespace SolEngine
{
    SolVulkanWindow::SolVulkanWindow(const std::string &winTitle, 
                                     const Vector2<uint32_t> &winDimensions)
    {
    }

    SolVulkanWindow::~SolVulkanWindow()
    {
        Dispose();
    }

    void SolVulkanWindow::Dispose()
    {
    }
}