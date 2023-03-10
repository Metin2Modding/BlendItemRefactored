// https://github.com/Thorek777
// https://github.com/Metin2Modding/BlendItemRefactored

#include "stdafx.h"
#include "affect.h"
#include "constants.h"
#include "BlendItem.hpp"
#include "locale_service.h"

#include <fstream>

bool CBlendItem::Load()
{
	if (!map.empty())
		map.clear();

	if (std::ifstream stream(LocaleService_GetBasePath() + "/Blend.json"); !stream)
	{
		sys_err("Config file doesn't exist.");
		return false;
	}
	else
	{
		try
		{
			const auto json =
				nlohmann::json::parse(stream, nullptr, true, true);

			stream.close();

			for (const auto& it : json.items())
			{
				blend schema = {};

				if (const auto& result = it.value().at("type"); result.is_number())
					schema.type = result;
				else
					schema.type = FN_get_apply_type(result.get<std::string>().c_str());

				if (const auto& result = it.value().at("value"); result.is_array())
					result.get_to(schema.value);
				else
					schema.value.push_back(result);

				if (const auto& result = it.value().at("duration"); result.is_array())
					result.get_to(schema.duration);
				else
					schema.duration.push_back(result);

				map.emplace(std::stoul(it.key()), schema);
			}

			if (map.empty())
			{
				sys_err("Container with items is empty.");
				return false;
			}
		}
		catch (const nlohmann::json::exception& exception)
		{
			sys_err("%s", exception.what());
			return false;
		}
	}

	return true;
}

bool CBlendItem::Find(const uint32_t item) const
{
	// If you using C++17, change that return to: map.find(item) != map.end().
	return map.contains(item);
}

void CBlendItem::Create(LPITEM item)
{
	if (!item || map.empty())
		return;

	if (const auto& it = map.find(item->GetVnum()); it != map.end())
	{
		const auto& [type, value, duration] = it->second;

		const auto applyValue =
			value.size() == 1 ? value.at(0) : value.at(number(0, value.size() - 1));

		const auto applyDuration =
			duration.size() == 1 ? duration.at(0) : duration.at(number(0, duration.size() - 1));

		item->SetSocket(0, type);
		item->SetSocket(1, applyValue);
		item->SetSocket(2, applyDuration);
	}
}

uint64_t CBlendItem::GetItems() const
{
	return map.size();
}
