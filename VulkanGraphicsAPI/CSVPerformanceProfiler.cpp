#include "CSVPerformanceProfiler.hpp"

namespace SolEngine::IO
{
    CSVPerformanceProfiler::CSVPerformanceProfiler(const std::string& workingDir)
        : _workingDir(workingDir)
    {
        GenerateFilenameWithTimestamp();
        InitCSV();
    }

    bool CSVPerformanceProfiler::LogToCSV(const DiagnosticData& diagnosticData)
    {
        const std::string filepath = _workingDir + _filename;

        std::ofstream fileStream;
        fileStream.open(filepath, std::ios_base::app);  // Open log file to append

        if (!fileStream.is_open())
        {
            fileStream.close();

            printf_s("Failed to open file: %s", filepath.c_str());

            return false;
        }

        fileStream.close();

        return true;
    }

    void CSVPerformanceProfiler::GenerateFilenameWithTimestamp()
    {
        const time_t time = std::time(NULL);                        // Retrieve the current timestamp
        tm localTime;
        ::localtime_s(&localTime, &time);

        std::ostringstream oss;
        oss << std::put_time(&localTime, CSV_TIMESTAMP_FORMAT);     // Format timestamp into string

        _filename = "3DCA_" + oss.str() + CSV_EXT;                  // Generate filename with timestamp
        _filepath = _workingDir + "\\PerformanceLog\\" + _filename; // Generate filepath
    }

    bool CSVPerformanceProfiler::InitCSV()
    {
        std::ofstream fileStream;
        fileStream.open(_filepath, std::fstream::out);  // Open log file to append

        if (!fileStream.is_open())
        {
            fileStream.close();

            printf_s("Failed to initialise CSV file: %s", _filepath.c_str());

            return false;
        }

        // Add CSV headers
        fileStream << CSV_HEADERS << std::endl;

        fileStream.close();

        return true;
    }
}