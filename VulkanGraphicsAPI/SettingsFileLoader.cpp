#include "SettingsFileLoader.hpp"

namespace SolEngine::Settings
{
    void SettingsFileLoader::LoadSettingsFromFile(const char* filepath,
                                                  SettingsBundle* pOutSettings)
    {
        *pOutSettings = SettingsBundle{};

        if (!pOutSettings->DeserializeFromFile(filepath))
        {
            printf_s("Failed to load settings from: %s", filepath);

            return;
        }

        printf_s("Successfully loaded settings from: %s", filepath);

        onFileLoadedEvent.Invoke(*pOutSettings);
    }
}