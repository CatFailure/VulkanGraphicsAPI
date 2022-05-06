#include "CSVPerformanceProfiler.hpp"

namespace SolEngine::IO
{
    CSVPerformanceProfiler::CSVPerformanceProfiler(const std::string& workingDir, 
                                                   DiagnosticData& rDiagnosticData,
                                                   SettingsBundle& rSettings)
        : _workingDir(workingDir),
          _rDiagnosticData(rDiagnosticData),
          _rSettings(rSettings)
    {
        GenerateFilenameWithTimestamp();
    }

    bool CSVPerformanceProfiler::LogSnapshot()
    {
        WriteDynamicDataRow();
        WriteNewLine();

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
        _outputDirectory = _workingDir + OUTPUT_FOLDER_NAME;    // Generate output dir
        _outputFilepath = _outputDirectory + _filename;         // Generate filepath
    }

    bool CSVPerformanceProfiler::CreateOutputDirectory()
    {
        // Create a "win api" friendly directory path string
        const std::wstring outputDirPath(_outputDirectory.begin(), 
                                         _outputDirectory.end());

        return CreateDirectory(outputDirPath.c_str(), NULL) || 
               GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool CSVPerformanceProfiler::Init()
    {
        _recordedEntriesCount = 0;

        if (!CreateOutputDirectory())
        {
            printf_s("Failed to Create Output Directory: %s",
                     _outputDirectory.c_str());

            return false;
        }

        WriteStaticHeaders();
        WriteNewLine();
        WriteStaticDataRow();
        WriteNewLine();
        WriteNewLine();
        WriteDynamicHeaders();
        WriteNewLine();

        return true;
    }

    bool CSVPerformanceProfiler::OpenCSV(const std::ios_base::openmode& mode,
                                         const OpenCSVCallback_t& callback)
    {
        std::ofstream fileStream;
        fileStream.open(_outputFilepath, mode);  // Open log file to append

        if (!fileStream.is_open())
        {
            fileStream.close();

            printf_s("Failed to Create CSV file: %s", 
                     _outputFilepath.c_str());

            return false;
        }

        callback(fileStream);

        fileStream.close();

        return true;
    }

    void CSVPerformanceProfiler::WriteNewLine()
    {
        OpenCSV(std::ios_base::app, 
                [](std::ofstream& rFilestream)
                {
                    rFilestream << std::endl;
                });
    }

    void CSVPerformanceProfiler::WriteStaticHeaders()
    {
        OpenCSV(std::ios_base::app, 
                [](std::ofstream& rFilestream)
                {
                    rFilestream << CSV_HEADERS_STATIC_DATA;
                });
    }

    void CSVPerformanceProfiler::WriteStaticDataRow()
    {
        SimulationSettings& rSimSettings        = _rSettings.simulationSettings;
        GameOfLifeSettings& rGameOfLifeSettings = _rSettings.gameOfLifeSettings;
        const glm::uvec3&   gridDimensions      = _rSettings.gridSettings.dimensions;

        std::string configuration;

#if _DEBUG || _DEBUG_LAPTOP
        configuration = "DEBUG";
#else
        configuration = "RELEASE";
#endif

        const std::string gridDimensions_string = std::format("(x: {} y: {} z: {})", 
                                                              gridDimensions.x,
                                                              gridDimensions.y,
                                                              gridDimensions.z);

        const std::string csvRow = std::format(CSV_ROW_FORMAT_STATIC_DATA,
                                               configuration,
                                               rSimSettings.seed, 
                                               gridDimensions_string, 
                                               _rDiagnosticData.GetTotalMemoryAllocatedBytes());

        OpenCSV(std::ios_base::app, 
                [&csvRow](std::ofstream& rFilestream)
                {
                    rFilestream << csvRow;
                });
    }

    void CSVPerformanceProfiler::WriteDynamicHeaders()
    {
        OpenCSV(std::ios_base::app, 
                [](std::ofstream& rFilestream)
                {
                    rFilestream << CSV_HEADERS_DYNAMIC_DATA;
                });
    }

    void CSVPerformanceProfiler::WriteDynamicDataRow()
    {
        if (IsMaxEntriesReached())
        {
            // Prevent more entries being added.
            return;
        }

        SimulationSettings& rSimSettings        = _rSettings.simulationSettings;
        GameOfLifeSettings& rGameOfLifeSettings = _rSettings.gameOfLifeSettings;
        const glm::uvec3&   gridDimensions      = _rSettings.gridSettings.dimensions;

        const std::string neighbourhoodType = 
            rGameOfLifeSettings.neighbourhoodType == NeighbourhoodType::VON_NEUMANN ?
                "VON_NEUMANN" : "MOORE";

        const std::string csvRow = std::format(CSV_ROW_FORMAT_DYNAMIC_DATA,
                                               rSimSettings.generation, 
                                               neighbourhoodType,
                                               rGameOfLifeSettings.underpopulationCount,
                                               rGameOfLifeSettings.overpopulationCount,
                                               rGameOfLifeSettings.reproductionCount,
                                               _rDiagnosticData.triCount,
                                               _rDiagnosticData.GetMemoryUsedPercentage(),
                                               _rDiagnosticData.deltaTimeSeconds,
                                               _rDiagnosticData.totalTimeSeconds);

        OpenCSV(std::ios_base::app, 
                [&csvRow](std::ofstream& rFilestream) 
                {
                    rFilestream << csvRow;
                });

        ++_recordedEntriesCount;
    }
}