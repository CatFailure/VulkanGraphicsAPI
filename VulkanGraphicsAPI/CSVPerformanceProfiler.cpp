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
        const time_t time = std::time(NULL);                    // Retrieve the current timestamp
        tm localTime;
        ::localtime_s(&localTime, &time);

        std::ostringstream oss;
        oss << std::put_time(&localTime, CSV_TIMESTAMP_FORMAT); // Format timestamp into string

        _filename = "3DCA_" + oss.str() + CSV_EXT;              // Generate filename with timestamp
        _outputDirectory = _workingDir + "\\PerformanceLog\\";  // Generate output dir
        _outputFilepath = _outputDirectory + _filename;         // Generate filepath
    }

    bool CSVPerformanceProfiler::CreateOutputDirectory()
    {
        const std::wstring outputDirPath(_outputDirectory.begin(), 
                                         _outputDirectory.end());

        return CreateDirectory(outputDirPath.c_str(), NULL) || 
               GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool CSVPerformanceProfiler::InitCSV()
    {
        if (!CreateOutputDirectory())
        {
            printf_s("Failed to Create Output Directory: %s",
                     _outputDirectory.c_str());

            return false;
        }

        std::ofstream fileStream;
        fileStream.open(_outputFilepath, std::fstream::out);  // Open log file to append

        if (!fileStream.is_open())
        {
            fileStream.close();

            printf_s("Failed to Create CSV file: %s", 
                     _outputFilepath.c_str());

            return false;
        }

        // Add CSV headers
        fileStream << CSV_HEADERS << std::endl;

        fileStream.close();

        return true;
    }
}