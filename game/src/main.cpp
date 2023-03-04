// When you deleted all results related with old system, add:
#include "BlendItem.hpp"

// Before:
	if (!start(argc, argv))
		return 0;

// Add:
	CBlendItem blendItem;

// And before:
	while (idle());

// Add:
	if (!g_bAuthServer && !CBlendItem::instance().Load())
		thecore_shutdown();