// https://github.com/Thorek777
// https://github.com/Metin2Modding/BlendItemRefactored

#pragma once

#include "item.h"

class CBlendItem : public singleton<CBlendItem>
{
	// Below structure will be a config for a container.
	struct blend
	{
		uint32_t type = 0;
		std::vector<uint32_t> value;
		std::vector<uint32_t> duration;
	}; std::unordered_map<uint32_t, blend> items;

public:
	// Function that allows to initialize items.
	bool Load();

	// Function that allows to find item identifier in initialized container.
	bool Find(uint32_t item);

	// Function that allows to create item based on initialized container.
	void Create(LPITEM item);

	// Function that allows to return size of initialized container.
	uint64_t GetItems() noexcept;
};
