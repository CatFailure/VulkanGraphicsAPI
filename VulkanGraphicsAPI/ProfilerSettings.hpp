#pragma once
#include "ISerializable.hpp"
#include "Helpers.hpp"

using namespace Utility;
using namespace SolEngine::Interface;

namespace SolEngine::Settings
{
    struct ProfilerSettings : public ISerializable
    {
        virtual bool Deserialize(const rapidjson::Value& obj) override 
        {
            if (!JsonHasMembers(obj, 
                                PROP_MAX_RECORD_COUNT))
            {
                return false;
            }

            maxRecordCount = obj[PROP_MAX_RECORD_COUNT].GetInt();

            return true;
        };

        int maxRecordCount{ -1 };   // Values below 0 disable recording

    private:
        static constexpr const char* PROP_MAX_RECORD_COUNT{ "max_record_count" };
    };
}
