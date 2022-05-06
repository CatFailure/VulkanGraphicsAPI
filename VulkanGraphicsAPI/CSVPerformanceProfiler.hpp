#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <sstream>
#include <fstream>
#include "DiagnosticData.hpp"

using namespace SolEngine::Data;

namespace SolEngine::IO
{
    class CSVPerformanceProfiler
    {
    public:
        CSVPerformanceProfiler(const std::string& workingDir);

        bool LogToCSV(const DiagnosticData& diagnosticData);

    private:
        static constexpr const char* CSV_EXT             { ".csv" };
        static constexpr const char* CSV_HEADERS         { "Seed,Generation,Grid Dimensions,Neighbourhood Type,Underpop.,Overpop.,Repro.,Delta Time" };
        static constexpr const char* CSV_TIMESTAMP_FORMAT{ "%d-%m-%Y_%H-%M-%S" };

        void GenerateFilenameWithTimestamp();
        bool InitCSV();

        const std::string& _workingDir;
        std::string _filename;
        std::string _filepath;
    };
}
