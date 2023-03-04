// When you deleted all results related with old system, add:
#include "BlendItem.hpp"

// And under:
			case 'c':
				Cube_init();
				break;

// Add:
			case 'b':
				{
					if (!CBlendItem::instance().Load())
					{
						ch->ChatPacket(CHAT_TYPE_INFO,
							"Blend wasn't loaded properly, check syserr!");
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO,
							"Blend was loaded, total items: %llu", CBlendItem::instance().GetItems());
					}
				}
				break;