#pragma once
#include "SolEvent.hpp"
#include "SimulationState.hpp"
#include "ISerializable.hpp"
#include "Helpers.hpp"

using namespace Utility;
using namespace SolEngine::Enumeration;
using namespace SolEngine::Events;
using namespace SolEngine::Interface;

namespace SolEngine::Settings
{
	struct SimulationSettings : public ISerializable
	{
		virtual bool Deserialize(const rapidjson::Value& obj) override
		{
			if (!JsonHasMembers(obj, 
								PROP_SEED,
								PROP_SPEED))
			{
				DBG_ASSERT_MSG(false, "[SimulationSettings] Object Missing Members!");

				return false;
			}

			seed  = obj[PROP_SEED].GetInt();
			speed = obj[PROP_SPEED].GetFloat();

			return true;    // OK!
		}

		void ResetGeneration()
		{
			generation = 0U;
		}

		void ResetSeed()
		{
			SimulationSettings defaultSettings{};
			seed = defaultSettings.seed;
		}

		void ResetSpeed()
		{
			SimulationSettings defaultSettings{};

			speed = defaultSettings.speed;
			onSimulationSpeedChangedEvent.Invoke(speed);
		}

		bool IsSimulationPlaying() const { return state == SimulationState::PLAY; }

		SolEvent<float> onSimulationSpeedChangedEvent;

		bool isSimulationResetRequested{ false };

		int				seed	  { 0 };
		size_t			generation{ 0U };
		float			speed	  { 0.1f };
		SimulationState state	  { SimulationState::PAUSED };

	private:
		static constexpr const char* PROP_SEED { "seed" };
		static constexpr const char* PROP_SPEED{ "speed" };
	};
}