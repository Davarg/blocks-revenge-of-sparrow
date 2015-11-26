#include "ConstantsRegistry.h"

ConstantsRegistry ConstantsRegistry::_registry;

void ConstantsRegistry::setValueForKey(constants constant, float32 value) {
	CCASSERT(!_registry._locked, "Constants are locked");
	_registry._values[constant] = value;
}

float32 ConstantsRegistry::getValueForKey(constants constant) {
	return _registry._values[constant];
}