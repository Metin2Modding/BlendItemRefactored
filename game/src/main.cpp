// When you deleted all results related with old system, add:
#include "BlendItem.hpp"

// Next, add:
	CBlendItem blendItem;

// Before this condition:
	if (!start(argc, argv))
		return 0;

// Then find:
	while (idle());

// And before add:
	if (!g_bAuthServer && !CBlendItem::instance().Load())
		thecore_shutdown();