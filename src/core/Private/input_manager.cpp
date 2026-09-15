#include "input_manager.h"

#include <algorithm>
#include <nlohmann/json.hpp>

#include "../filesystem.h"

static std::vector<int> keys_pressed;
static std::vector<int> keys_just_pressed;
static std::vector<int> keys_just_released;

static std::unordered_map<std::string, std::vector<int>> action_mappings;
static std::unordered_map<std::string, std::vector<hge::action_t>> axis_mappings;

namespace hge::priv::input
{
	void LoadConfigFile(const char *path)
	{
		size_t size;
		std::string data = hn::GetFileContent(path, &size, true);
		nlohmann::json j = nlohmann::json::parse(data, nullptr, false);
		if (j.is_discarded())
		{
			//LOG_ERROR("LoadConfig, invalid JSON");
			return;
		}

		// action-mappings
		if (j.contains("action-mappings"))
		{
			for (const auto& [name, keys] : j["action-mappings"].items())
			{
				std::vector<int> keycodes;
				for (const auto& k : keys)
					keycodes.emplace_back(std::stoi(k.get<std::string>()));
				action_mappings[name] = std::move(keycodes);
			}
		}

		// axis-mappings
		if (j.contains("axis-mappings"))
		{
			for (const auto& [name, actions] : j["axis-mappings"].items())
			{
				std::vector<hge::action_t> acts;
				for (const auto& a : actions)
				{
					hge::action_t act;
					act.key   = std::stoi(a["key"].get<std::string>());
					act.value = a["scale"].get<float>();
					acts.emplace_back(act);
				}
				axis_mappings[name] = std::move(acts);
			}
		}
	}

	void Tick()
	{
		keys_just_pressed.clear();
		keys_just_released.clear();
	}

	void InjectKeyDown(int key)
	{
		bool contains = std::find(keys_pressed.begin(), keys_pressed.end(), key) != keys_pressed.end();
		if (!contains)
		{
			keys_pressed.emplace_back(key);
			keys_just_pressed.emplace_back(key);
		}
	}

	void InjectKeyUp(int key)
	{
		auto it = std::find(keys_pressed.begin(), keys_pressed.end(), key);
		if (it != keys_pressed.end())
		{
			//if key was already pressed
			keys_pressed.erase(it);
			keys_just_released.emplace_back(key);
		}
	}

	const std::vector<int> &GetPressedKeys()
	{
		return keys_pressed;
	}

	const std::vector<int> &GetJustPressedKeys()
	{
		return keys_just_pressed;
	}

	const std::vector<int> &GetJustReleasedKeys()
	{
		return keys_just_released;
	}


	const std::vector<int>& GetActionKeys(const char* name)
	{
		static const std::vector<int> empty;
		auto it = action_mappings.find(name);
		if (it == action_mappings.end())
		{
			//LOG_ERROR("GetActionKeys, unknown action");
			return empty;
		}
		return it->second;
	}

	const std::vector<action_t>& GetAxisActions(const char* name)
	{
		static const std::vector<action_t> empty;
		auto it = axis_mappings.find(name);
		if (it == axis_mappings.end())
		{
			//LOG_ERROR("GetAxisActions, unknown axis");
			return empty;
		}
		return it->second;
	}
}