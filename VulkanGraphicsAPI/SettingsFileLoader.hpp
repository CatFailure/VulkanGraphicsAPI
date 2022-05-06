#pragma once
#include "SolEvent.hpp"
#include "SettingsBundle.hpp"
#include "Constants.hpp"
#include "Singleton.hpp"

using namespace Utility;
using namespace SolEngine::Events;

namespace SolEngine::Settings
{
    struct SettingsFileLoader : public Singleton<SettingsFileLoader>
    {
        void LoadSettingsFromFile(const char* filepath, SettingsBundle* pOutSettings);

        SolEvent<SettingsBundle> onFileLoadedEvent;
    };
}
