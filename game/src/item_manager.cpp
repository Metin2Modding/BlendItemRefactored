// When you deleted all results related with old system, add:
#include "BlendItem.hpp"

// And replace:
			if (Blend_Item_find(item->GetVnum()))
			{
				Blend_Item_set_value(item);
				return item;
			}

// With:
			if (CBlendItem::instance().Find(item->GetVnum()))
			{
				CBlendItem::instance().Create(item);
				return item;
			}