// https://github.com/Thorek777
// https://github.com/Metin2Modding/BlendItemRefactored

#include "stdafx.h"
#include "affect.h"
#include "constants.h"
#include "BlendItem.hpp"
#include "locale_service.h"

#include <fstream>
#include <json.hpp>

// Function that allows to initialize items.
bool CBlendItem::Load()
{
	// Pay attention to fact that container with items will be cleared if isn't empty.
	// This operation is neccessary if you want to reloading config in game directly.
	// So if e.g. JSON will be not parsed correctly, container with items will be cleared.
	// Player who will buy item from shop, will not receive item with attributes.
	if (!items.empty())
		items.clear();

	// It's time to load JSON file.
	std::ifstream stream(LocaleService_GetBasePath() + "/Blend.json");

	if (!stream)
	{
		// At this point, file doesn't exist.
		// I created simple log to inform about it.
		sys_err("Config file doesn't exist.");
		return false;
	}
	else
	{
		// If file exist, I used method to catch potential JSON exceptions.
		// It's so useful, because I can inform about errors or something else.
		try
		{
			// I can try to parse stream.
			const auto json = nlohmann::json::parse(stream, nullptr, true, true);

			// And I can close this stream because JSON was parsed.
			stream.close();

			for (const auto& it : json.items())
			{
				// So, it's time to create schema based on config in header file.
				blend schema = {};

				// I can try now to search attribute.
				// If it will be not a number, I can replace text with identifier.
				const auto& result = it.value().at("type");

				if (result.is_number())
					schema.type = result;
				else
					schema.type = FN_get_apply_type(result.get<std::string>().c_str());

				// If I found bonus value, I can push it to container.
				// Otherwise, if I found bonus values, I can copy values to container.
				const auto& result = it.value().at("value");

				if (result.is_array())
					result.get_to(schema.value);
				else
					schema.value.push_back(result);

				// If I found bonus duration, I can push it to container.
				// Otherwise, if I found bonus durations, I can copy values to container.
				const auto& result = it.value().at("duration");

				if (result.is_array())
					result.get_to(schema.duration);
				else
					schema.duration.push_back(result);

				// I think that schema will be filled.
				// It's time to push it to container with items.
				items.emplace(std::stoul(it.key()), schema);
			}

			// If items container is empty, it's neccessary to make error.
			if (items.empty())
			{
				sys_err("Container with items is empty.");
				return false;
			}
		}
		catch (const nlohmann::json::exception& exception)
		{
			// At this point, something went wrong while JSON parsing probably.
			// One and only thing I can do is inform about exception.
			sys_err("%s", exception.what());
			return false;
		}
	}

	return true;
}

// Function that allows to find item identifier in initialized config.
bool CBlendItem::Find(const uint32_t item) const
{
	return items.find(item) != items.end();
}

// Function that allows to create item based on initialized config.
void CBlendItem::Create(LPITEM item)
{
	// If item pointer is null or container with items is empty, return false.
	if (!item || items.empty())
		return;

	const auto& it = items.find(item->GetVnum());

	if (it != items.end())
	{
		const auto& it2 = it->second;

		const auto applyValue =
			it2.value.size() == 1 ? it2.value.at(0) : it2.value.at(number(0, it2.value.size() - 1));

		const auto applyDuration =
			it2.duration.size() == 1 ? it2.duration.at(0) : it2.duration.at(number(0, it2.duration.size() - 1));

		item->SetSocket(0, static_cast<int32_t>(it2.type));
		item->SetSocket(1, static_cast<int32_t>(applyValue));
		item->SetSocket(2, static_cast<int32_t>(applyDuration));
	}
}

// Function that allows to return size of initialized config.
uint64_t CBlendItem::GetItems() const
{
	return items.size();
}
