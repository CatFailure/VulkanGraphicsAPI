#include "SettingsFileLoader.hpp"

namespace SolEngine::IO
{
    void SettingsFileLoader::LoadSettingsFromFile(const char* filepath,
                                                  UserSettings* pOutSettings)
    {
        *pOutSettings = UserSettings{};

        if (!pOutSettings->DeserializeFromFile(filepath))
        {
            printf_s("Failed to load settings from: %s\n", filepath);

            return;
        }

        printf_s("Successfully loaded settings from: %s\n", filepath);

        onFileLoadedEvent.Invoke(*pOutSettings);
    }
}