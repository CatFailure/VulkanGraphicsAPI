#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <format>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <functional>

#include "DiagnosticData.hpp"
#include "UserSettings.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Settings;

namespace SolEngine::IO
{
    class CSVPerformanceProfiler
    {
    public:
        typedef std::function<void(std::ofstream&)> OpenCSVCallback_t;

        CSVPerformanceProfiler(const std::string& workingDir, DiagnosticData& rDiagnosticData, UserSettings& rSettings);

        bool Init();
        bool LogSnapshot();
        bool IsMaxEntriesReached() const { return !(_recordedEntriesCount < _rSettings.profilerSettings.maxRecordCount); }

    private:
        static constexpr const char* CSV_EXT                    { ".csv" };
        static constexpr const char* CSV_HEADERS_STATIC_DATA    { "Configuration,Seed,Grid Dimensions,Mem. Allocated (MiB)" };
        static constexpr const char* CSV_HEADERS_DYNAMIC_DATA   { "Generation,Neighbourhood Type,Underpopulation,Overpopulation,Reproduction,Tri. Count,Mem. Used (%),Delta Time (s),Total Time (s)" };
        static constexpr const char* CSV_ROW_FORMAT_STATIC_DATA {"{},{},{},{}"};
        static constexpr const char* CSV_ROW_FORMAT_DYNAMIC_DATA{"{},{},{},{},{},{},{},{},{}"};
        static constexpr const char* CSV_TIMESTAMP_FORMAT       { "%d-%m-%Y_%H-%M-%S" };
        static constexpr const char* OUTPUT_FOLDER_NAME         { "\\CSV\\" };

        void GenerateFilenameWithTimestamp();
        bool CreateOutputDirectory();
        bool OpenCSV(const std::ios_base::openmode& mode, const OpenCSVCallback_t& callback);
        void WriteNewLine();
        void WriteStaticHeaders();
        void WriteStaticDataRow();
        void WriteDynamicHeaders();
        void WriteDynamicDataRow();

        const std::string& _workingDir;
        DiagnosticData&    _rDiagnosticData;
        UserSettings&      _rSettings;

        std::string _filename;
        std::string _outputDirectory;
        std::string _outputFilepath;

        int _recordedEntriesCount{ 0 };
    };
}
