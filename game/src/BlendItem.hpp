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

#pragma once

#include "item.h"

class CBlendItem : public singleton<CBlendItem>
{
	// Below structure will be a config for a container.
	struct blend
	{
		uint32_t item = 0;
		uint32_t type = 0;
		std::vector<uint32_t> value;
		std::vector<uint32_t> duration;
	}; std::vector<blend> items;

public:
	// Function that allows to initialize items.
	bool Load();

	// Function that allows to find item identifier in initialized container.
	bool Find(uint32_t item) noexcept;

	// Function that allows to create item based on initialized container.
	void Create(LPITEM item);

	// Function that allows to return size of initialized container.
	uint64_t GetItems() noexcept;
};
