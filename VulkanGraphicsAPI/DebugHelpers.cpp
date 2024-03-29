#include "DebugHelpers.hpp"

namespace Utility
{
    void DebugHelpers::DPrintf(const char *format, ...)
    {
        static char buffer[2048]{ 0 };          // Temp buffer
        va_list params;                         // Variable argument params start

        const char *logFileName("log.txt");     
        const char *mode("a+");

        va_start(params, format);               // Try to print in allocated space
        vsprintf_s(buffer, format, params);     // Format data into a string buffer
        va_end(params);

#ifdef ENABLE_LOGGING                                           // Write the info out to a txt file
        FILE *pFile = nullptr;
        errno_t result = fopen_s(&pFile, logFileName, mode);    // Open file for writing

        DBG_ASSERT_MSG(pFile || result,
                       "Log file could not be opened.");

        fprintf(pFile, "%s", buffer);   // Write to file
        fclose(pFile);                  // Close file
#endif // ENABLE_LOGGING

        OutputDebugStringA(buffer);     // Output to the Visual Studio Window
    }
}