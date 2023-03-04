//  ____  _                _ ___ _
// | __ )| | ___ _ __   __| |_ _| |_ ___ _ __ ___
// |  _ \| |/ _ \ '_ \ / _` || || __/ _ \ '_ ` _ \
// | |_) | |  __/ | | | (_| || || ||  __/ | | | | |
// |____/|_|\___|_| |_|\__,_|___|\__\___|_| |_| |_|
//
// Author:
//     Thorek
//
// Last modified:
//     04.03.2023
//

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
	if (std::ifstream stream(LocaleService_GetBasePath() + "/Blend.json"); !stream)
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
				// My first item in schema is item identifier.
				// In JSON file this is the key.
				blend schema = { std::stoul(it.key()) };

				// I can try now to search attribute.
				// If it will be not a number, I can replace text with identifier.
				if (const auto& result = it.value().at("type"); result.is_number())
					schema.type = result;
				else
					schema.type = GetApplyType(result.get<std::string>().c_str());

				// If I found bonus value, I can push it to container.
				// Otherwise, if I found bonus values, I can copy values to container.
				if (const auto& result = it.value().at("value"); result.is_array())
					result.get_to(schema.value);
				else
					schema.value.push_back(result);

				// If I found bonus duration, I can push it to container.
				// Otherwise, if I found bonus durations, I can copy values to container.
				if (const auto& result = it.value().at("duration"); result.is_array())
					result.get_to(schema.duration);
				else
					schema.duration.push_back(result);

				// I think that schema will be filled.
				// It's time to push it to container with items.
				items.push_back(schema);
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
bool CBlendItem::Find(const uint32_t item) noexcept
{
	// If container with items is empty, return false.
	if (items.empty())
		return false;

	// Otherwise, check if item identifier from argument occurs in container.
	for (const auto& it : items)
		if (it.item == item)
			return true;

	return false;
}

// Function that allows to create item based on initialized config.
void CBlendItem::Create(LPITEM item)
{
	// If item pointer is null or container with items is empty, return false.
	if (!item || items.empty())
		return;

	for (const auto& it : items)
	{
		// Otherwise, check if item identifier from iterator is equal to identifier from pointer.
		if (it.item != item->GetVnum())
			continue;

		// If size of bonuses is only 1, set variable to this value.
		// Otherwise, do simple randomization.
		const auto applyValue =
			it.value.size() == 1 ? it.value.at(0) : it.value.at(number(0, it.value.size() - 1));

		// If size of durations is only 1, set variable to this value.
		// Otherwise, do simple randomization.
		const auto applyDuration =
			it.duration.size() == 1 ? it.duration.at(0) : it.duration.at(number(0, it.duration.size() - 1));

		// At the end, it's just enough to set sockets.
		item->SetSocket(0, it.type);
		item->SetSocket(1, applyValue);

#ifndef ENABLE_EXTENDED_BLEND_AFFECT_SYSTEM
		item->SetSocket(2, applyDuration);
#else
		item->SetSocket(2, applyDuration ? applyDuration : INFINITE_AFFECT_DURATION);
#endif
	}
}

// Function that allows to return size of initialized config.
uint64_t CBlendItem::GetItems() noexcept
{
	return items.size();
}
