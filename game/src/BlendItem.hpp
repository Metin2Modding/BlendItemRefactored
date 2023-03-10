// https://github.com/Thorek777
// https://github.com/Metin2Modding/BlendItemRefactored

#pragma once

#include "item.h"

class CBlendItem final : public singleton<CBlendItem>
{
	struct blend
	{
		int32_t type = 0;
		std::vector<int32_t> value;
		std::vector<int32_t> duration;
	}; std::unordered_map<uint32_t, blend> map;

public:
	bool Load();

	[[nodiscard]]
	bool Find(uint32_t item) const;

	void Create(LPITEM item);

	[[nodiscard]]
	uint64_t GetItems() const;
};
