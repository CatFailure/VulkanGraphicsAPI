#pragma once
#include "SolEvent.hpp"
#include "Typedefs.hpp"
#include "NeighbourhoodType.hpp"
#include "ISerializable.hpp"
#include "Helpers.hpp"

using namespace Utility;
using namespace SolEngine::Events;
using namespace SolEngine::Enumeration;

namespace SolEngine::Settings
{
	struct GameOfLifeSettings : public ISerializable
	{
		static constexpr NeighbourCount_t DEFAULT_UNDERPOPULATION_COUNT_MOORE	   { 7U };
		static constexpr NeighbourCount_t DEFAULT_OVERPOPULATION_COUNT_MOORE	   { 13U };
		static constexpr NeighbourCount_t DEFAULT_REPRODUCTION_COUNT_MOORE		   { 12U };

		static constexpr NeighbourCount_t DEFAULT_UNDERPOPULATION_COUNT_VON_NEUMANN{ 2U };
		static constexpr NeighbourCount_t DEFAULT_OVERPOPULATION_COUNT_VON_NEUMANN { 3U };
		static constexpr NeighbourCount_t DEFAULT_REPRODUCTION_COUNT_VON_NEUMANN   { 3U };

        virtual bool Deserialize(const rapidjson::Value& obj) override
        {
			if (!JsonHasMembers(obj, 
								PROP_NEIGHBOURHOOD_TYPE, 
								PROP_UNDERPOPULATION, 
								PROP_OVERPOPULATION, 
								PROP_REPRODUCTION))
			{
				DBG_ASSERT_MSG(false, "[GameOfLifeSettings] Object Missing Members!");

				return false;
			}

			neighbourhoodType	 = (NeighbourhoodType)obj[PROP_NEIGHBOURHOOD_TYPE].GetUint();
			underpopulationCount = (NeighbourCount_t)obj[PROP_UNDERPOPULATION].GetUint();
			overpopulationCount  = (NeighbourCount_t)obj[PROP_OVERPOPULATION].GetUint();
			reproductionCount	 = (NeighbourCount_t)obj[PROP_REPRODUCTION].GetUint();

            return true;    // OK!
        }

		void Reset()
		{
			underpopulationCount = GetDefaultUnderpopulationCount();
			overpopulationCount  = GetDefaultOverpopulationCount();
			reproductionCount    = GetDefaultReproductionCount();
		}

		NeighbourCount_t GetDefaultUnderpopulationCount() const { return neighbourhoodType == NeighbourhoodType::MOORE ? DEFAULT_UNDERPOPULATION_COUNT_MOORE : DEFAULT_UNDERPOPULATION_COUNT_VON_NEUMANN; }
		NeighbourCount_t GetDefaultOverpopulationCount()  const { return neighbourhoodType == NeighbourhoodType::MOORE ? DEFAULT_OVERPOPULATION_COUNT_MOORE : DEFAULT_OVERPOPULATION_COUNT_VON_NEUMANN; }
		NeighbourCount_t GetDefaultReproductionCount()    const { return neighbourhoodType == NeighbourhoodType::MOORE ? DEFAULT_REPRODUCTION_COUNT_MOORE : DEFAULT_REPRODUCTION_COUNT_VON_NEUMANN; }

		NeighbourhoodType neighbourhoodType	  { NeighbourhoodType::MOORE };	// What cells are considered "Neighbours"?
		NeighbourCount_t  underpopulationCount{ 7U };						// Min number of live neighbours to keep a cell alive.
		NeighbourCount_t  overpopulationCount { 13U };						// Max number of live neighbours to keep a cell alive.
		NeighbourCount_t  reproductionCount   { 12U };						// Exact number of live neighbours to reproduce a cell.

		SolEvent<> onNeighbourhoodTypeChangedEvent;

	private:
		static constexpr const char* PROP_NEIGHBOURHOOD_TYPE{ "neighbourhood_type" };
		static constexpr const char* PROP_UNDERPOPULATION	{ "underpopulation" };
		static constexpr const char* PROP_OVERPOPULATION	{ "overpopulation" };
		static constexpr const char* PROP_REPRODUCTION		{ "reproduction" };
	};
}