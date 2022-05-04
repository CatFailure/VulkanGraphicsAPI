#pragma once
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <string>

#include "Constants.hpp"

using namespace SolEngine::Data;

namespace SolEngine::Interface
{
    struct ISerializable
    {
        bool DeserializeFromFile(const std::string& filepath)
        {
            FILE* pFile;
            char* fileBuffer = (char*)malloc(FILE_BUFFER_SIZE);

            if (fopen_s(&pFile, 
                        filepath.c_str(), 
                        "rb") == 0 ||
                pFile == NULL)
            {
                free(fileBuffer);

                return false;
            }

            rapidjson::FileReadStream readStream(pFile,
                                                 fileBuffer,
                                                 FILE_BUFFER_SIZE);
            rapidjson::Document document{};

            document.ParseStream(readStream);

            // Clean up buffer and close stream
            free(fileBuffer);
            fclose(pFile);

            return Deserialize(document.GetObj());
        }

        virtual bool Deserialize(const rapidjson::Value& obj) = 0;
    };
}
