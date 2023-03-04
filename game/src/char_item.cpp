// When you deleted all results related with old system, add:
#include "BlendItem.hpp"

// And replace:
		if (Blend_Item_find(item->GetVnum()))

// With:
		if (CBlendItem::instance().Find(item->GetVnum()))