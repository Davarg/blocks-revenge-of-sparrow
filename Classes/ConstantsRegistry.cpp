#include "ConstantsRegistry.h"

void ConstantsRegistry::setValueForKey(constants constant, float32 value) {
	CCASSERT(!isLocked, "Constants are locked.");
	constantsRegistry._values[constant] = value;
}

float32 ConstantsRegistry::getValueForKey(constants constant) {
	return constantsRegistry._values[constant];
}